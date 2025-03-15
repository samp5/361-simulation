#include "../../sim.h"
#include "../log_macros.h"
#include "state.h"
#include "sync.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned long CUSTOMER_EAT_DELAY = 2000; // in microseconds
extern state *GLOBAL_STATE;
static customer *get_mut_customer(customer_id);
static table *get_mut_table(table_id id);
static void customer_start_to_eat(customer_id id);
static void *customer_eat_routine(void *id);
static int can_arrive(customer_status stat);
static int can_order(customer_status stat);
static int can_be_seated(customer_status stat);
static int is_waiting_for_food(customer_status stat);
static int is_eating(customer_status stat);

void inconsistent_state();

/*
 * NOTE: Lock.Customer must be held before this call and while the returned
 * customer is in scope
 */
static customer *get_mut_customer(customer_id id) {

  customer dummy;
  dummy.id = id;

  vector *customers = GLOBAL_STATE->customers;
  int customer_index = customers->find(customers, (void *)&dummy);

  if (customer_index == -1) {
    LOG("Customer with ID = %d does not exists... exiting", id);
    inconsistent_state();
  }
  customer *target;
  int _ = customers->get_mut_at(customers, customer_index, (void **)&target);
  return target;
}

/*
 * NOTE: The Lock.Tables must be held before this call and while the returned
 * pointer is in scope
 */
static table *get_mut_table(table_id id) {
  table dummy;
  dummy.id = id;

  vector *tables = GLOBAL_STATE->tables;
  int table_index = tables->find(tables, (void *)&dummy);

  if (table_index == -1) {
    LOG("Table with ID = %d does not exists... exiting", id);
    inconsistent_state();
  }

  table *target;
  int _ = tables->get_mut_at(tables, table_index, (void **)&target);
  return target;
}

/*
 * Check to see if a customer  arrived at the restaurant
 */
int has_arrived(customer_id id) {

  int locks = Customer;
  take(locks);

  LOG("Checking cutomer %d has arrived", id);
  customer *target = get_mut_customer(id);
  int arrived = !can_arrive(target->current_status);

  release(locks);

  return arrived;
}

/* TODO:
 * Leave the restaurant
 */
void leave(customer_id id) {}

/*
 * Eat borsht
 */
void eat(customer_id id) {
  int locks = Customer | Table;
  take(locks);

  LOG("Cutomer %d trying to eat", id);

  customer *target = get_mut_customer(id);

  if (is_eating(target->current_status)) {
    BAIL_AND_RELEASE("Customer %d tried to eat but is already busy eating!",
                     id);
    inconsistent_state();
  }

  if (!is_waiting_for_food(target->current_status)) {
    BAIL_AND_RELEASE(
        "Customer %d was not wating for food. A customer needs to be at a "
        "table and have ordered before they can eat.",
        id);
    inconsistent_state();
  }

  table *target_table = get_mut_table(target->table_id);

  if (target_table->borsht_bowls[target->preference] != 1) {
    BAIL_AND_RELEASE(
        "Customer %d is sitting at table %d. There are %d bowls of borsht type "
        "%d there. There must be exactly 1",
        target->id, target_table->id,
        target_table->borsht_bowls[target->preference], target->preference);
    inconsistent_state();
  } else {

    // update target state and table state
    target->current_status = target->current_status | Eating;

    customer_start_to_eat(target->id);

    LOG("Customer %d is starting to eat BORSHT", target->id);
  }

  release(locks);
}

/*
 * Detach a thread that will modify a cusotmers state after some delay
 */
static void customer_start_to_eat(customer_id id) {
  pthread_t tid;
  customer_id *id_ptr = malloc(sizeof(customer_id));
  *id_ptr = id;
  int creat_ret =
      pthread_create(&tid, NULL, customer_eat_routine, (void *)id_ptr);
  if (creat_ret != 0) {
    perror("pthread_create");
    exit(1);
  }
  pthread_detach(tid);
}

static void *customer_eat_routine(void *arg) {
  customer_id id = *(customer_id *)arg;
  usleep(CUSTOMER_EAT_DELAY);

  int locks = Customer | Table;
  take(locks);

  customer *custy = get_mut_customer(id);
  table *table = get_mut_table(id);

  custy->borsht_eaten += 1;
  table->borsht_bowls[custy->preference] -= 1;

  if (custy->current_status != (AtTable | Eating | Ordered)) {
    LOG("Customer %d had incorrect status in internal eat routine, this is a "
        "mistake.",
        custy->id);
    inconsistent_state();
  }

  custy->current_status = (AtTable | Ordered);

  release(locks);

  pthread_exit(NULL);
}

static int can_arrive(customer_status stat) { return stat == NotArrived; }

static int can_order(customer_status stat) { return stat == AtTable; }

static int can_be_seated(customer_status stat) { return stat == InQueue; }

static int is_waiting_for_food(customer_status stat) {
  return stat == (AtTable | Ordered);
}

static int is_eating(customer_status stat) {
  return (stat & AtTable) && (stat & Eating);
}
