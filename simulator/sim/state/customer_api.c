#include "../../sim.h"
#include "../log_macros.h"
#include "state.h"
#include "sync.h"
#include "util.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned long CUSTOMER_EAT_DELAY = 2000; // in microseconds
unsigned long CUSTOMER_ARRIVE_DELAY =
    10000; // in microseconds (10 ms)
           // will range from 50 to 100 ms in impl
extern state *GLOBAL_STATE;
/*
 * Customer routines
 */
static void run_customer_routine(customer_id id, void *(*routine)(void *));
static void *customer_arrive_routine(void *id);

/*
 * Customer status helper functions
 */
static int can_arrive(customer_status stat);
static int is_waiting_for_food(customer_status stat);
static int is_eating(customer_status stat);

// If I don't include this I get a weird warning?
extern int usleep(__useconds_t useconds);

/*
 * Check to see if a customer  arrived at the restaurant
 */
int has_arrived(customer_id id) {

  int locks = Global;
  take(locks);

  LOG("Checking cutomer %d has arrived", id);
  customer *target;
  if ((target = get_mut_customer(id)) == NULL) {
    BAIL_RET_RELEASE(-1, "Customer %d was NULL", id);
  }

  int arrived = !can_arrive(target->current_status);

  release(locks);

  return arrived;
}

/* TODO:
 * Leave the restaurant
 */
void leave(customer_id id) {
  int locks = Global;
  take(locks);

  LOG("Cutomer %d is leaving", id);

  customer *target;
  if ((target = get_mut_customer(id)) == NULL) {
    BAIL_AND_RELEASE("Customer %d was NULL", id);
  }

  if (target->borsht_desired != target->borsht_eaten) {
    BAIL_AND_RELEASE("Customer %d tried to leave but is still hungry! Ate %d "
                     "bowls but wanted %d",
                     id, target->borsht_eaten, target->borsht_desired);
  }

  // set table status to dirty
  table *target_table;
  if ((target_table = get_mut_table(target->table_id)) == NULL) {
    BAIL_AND_RELEASE("Table %d was NULL", target->table_id);
  }

  target_table->current_status = Dirty;

  // set status to left
  target->current_status = Left;
  target->table_id = -1;
  GLOBAL_STATE->num_customers_left += 1;

  release(locks);
}

/*
 * Eat borsht
 *
 * Pitfalls:
 * 1. Cutomer does not exist
 * 2. Customer is already eating
 * 3. Customer is not waiting for food  (not AtTable & Ordered)
 * 4. Customer is already full
 */
void eat(customer_id id) {
  int locks = Global;
  take(locks);

  LOG("Cutomer %d trying to eat", id);

  // 1. Cutomer does not exist
  customer *target;
  if ((target = get_mut_customer(id)) == NULL) {
    BAIL_AND_RELEASE("Customer %d was NULL", id);
  }

  // 2. Customer is already eating
  if (is_eating(target->current_status)) {
    BAIL_AND_RELEASE("Customer %d tried to eat but is already busy eating!",
                     id);
  }

  // 3. Customer is not waiting for food  (not AtTable & Ordered)
  if (!is_waiting_for_food(target->current_status)) {
    BAIL_AND_RELEASE(
        "Customer %d was not wating for food. A customer needs to be at a "
        "table and have ordered before they can eat.",
        id);
  }

  // 4. Customer is already full
  if (target->borsht_eaten >= target->borsht_desired) {
    BAIL_AND_RELEASE(
        "Customer %d is trying to eat borsht, it's the kind they like but they "
        "are so full! Wanted %d bowls, but got served one too many.",
        target->id, target->borsht_desired);
  }

  table *target_table;
  if ((target_table = get_mut_table(target->table_id)) == NULL) {
    BAIL_AND_RELEASE("ERROR IN INTERNAL DATASTRUCTURE: REPORT TO PROFESSOR: "
                     "Target table %d was NULL",
                     id);
  }

  if (target_table->borsht_bowls[target->preference] != 1) {
    BAIL_AND_RELEASE(
        "Customer %d is sitting at table %d. There are %d bowls of borsht type "
        "%d there. There must be exactly 1",
        target->id, target_table->id,
        target_table->borsht_bowls[target->preference], target->preference);
  }

  // update target state and table state
  target->current_status = target->current_status | Eating;

  LOG("Customer %d is starting to eat BORSHT", target->id);

  // eat
  usleep(CUSTOMER_EAT_DELAY);

  target->current_status = target->current_status ^ Eating;
  target_table->borsht_bowls[target->preference] -= 1;
  target->borsht_eaten += 1;

  LOG("Customer %d is done eating BORSHT", target->id);

  release(locks);
}

/*
 * Create a detached thread that modifies customer status from
 * NotArrived to InQueue after some semi-random amount of time
 */
void delay_customer_arrival(customer_id id) {
  run_customer_routine(id, customer_arrive_routine);
}

/*
 * Detach a thread that will modify a cusotmers state after some delay
 */
static void run_customer_routine(customer_id id, void *(*routine)(void *)) {
  pthread_t tid;
  customer_id *id_ptr = malloc(sizeof(customer_id));
  *id_ptr = id;
  int creat_ret = pthread_create(&tid, NULL, routine, (void *)id_ptr);
  if (creat_ret != 0) {
    perror("pthread_create");
    exit(1);
  }
  pthread_detach(tid);
}

static void *customer_arrive_routine(void *arg) {
  customer_id id = *(customer_id *)arg;
  usleep(CUSTOMER_ARRIVE_DELAY * (rand() % 10 + 5));

  int locks = Global;
  take(locks);

  customer *custy;
  if ((custy = get_mut_customer(id)) == NULL) {
    BAIL_RET_RELEASE(NULL, "Customer %d was NULL", id);
  }

  if (custy->current_status != NotArrived) {
    BAIL_RET_RELEASE(
        NULL,
        "Customer %d had an incorrect status in the internal arrival routine, "
        "this is a mistake.",
        custy->id);
  }

  add_customer_to_queue(custy->id);
  GLOBAL_STATE->num_customers_arrived += 1;

  release(locks);

  pthread_exit(NULL);
}

static int can_arrive(customer_status stat) { return stat == NotArrived; }

static int is_waiting_for_food(customer_status stat) {
  return stat == (AtTable | Ordered);
}

static int is_eating(customer_status stat) {
  return (stat & AtTable) && (stat & Eating);
}
