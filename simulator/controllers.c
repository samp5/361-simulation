#include "sim/controllers.h"
#include "restaurant.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>

pthread_cond_t cust_cond;
pthread_cond_t waiter_customer_cond;
pthread_cond_t waiter_cook_cond;
pthread_cond_t cook_cond;
pthread_mutex_t tex;

int seated = 0;
int order = 0;
int type = 0;
int order_ready = 0;
int food_at_table = 0;
int customer_done = 0;
int food_picked_up = 0;
int all_food_delivered = 0;
int customers_left = 0;

void initializer() {
  pthread_cond_init(&cust_cond, NULL);
  pthread_cond_init(&waiter_customer_cond, NULL);
  pthread_cond_init(&waiter_cook_cond, NULL);
  pthread_cond_init(&cook_cond, NULL);
  pthread_mutex_init(&tex, NULL);
}

void wait_staff_controller(waitstaff_id id) {
  customer_id *line;
  size_t c_len;

  get_waiting_line(&line, &c_len);

  if (c_len == 0) {
    return;
  }

  table_id *my_tables;
  size_t t_len;

  get_tables(id, &my_tables, &t_len);

  customer_id to_seat = line[0];

  seat(id, to_seat, my_tables[0]);

  borsht_type pref = intuit_preference(to_seat);

  int quantity;
  take_order(id, to_seat, &quantity);

  pthread_mutex_lock(&tex);

  order = quantity;
  type = pref;

  pthread_cond_signal(&cook_cond);

  for (int i = 0; i < quantity; i++) {

    while (!order_ready)
      pthread_cond_wait(&waiter_cook_cond, &tex);

    pick_up_borsht(id, pref, 1);
    food_picked_up = 1;
    order_ready = 0;

    pthread_cond_signal(&cook_cond);

    serve(id, my_tables[0], pref);

    food_at_table = 1;

    pthread_cond_signal(&cust_cond);
    while (food_at_table)
      pthread_cond_wait(&waiter_customer_cond, &tex);

    if (i == order - 1) {
      all_food_delivered = 1;
    }
    pthread_cond_signal(&cust_cond);
  }

  all_food_delivered = 1;

  pthread_mutex_unlock(&tex);
}

void cook_controller() {
  pthread_mutex_lock(&tex);

  while (!order) {
    pthread_cond_wait(&cook_cond, &tex);
    if (customers_left == NUM_CUSTOMERS) {
      pthread_mutex_unlock(&tex);
      return;
    }
  }

  for (int i = 0; i < order; i++) {
    prepare(type, 1);

    order_ready = 1;
    food_picked_up = 0;

    pthread_cond_signal(&waiter_cook_cond);
    while (!food_picked_up)
      pthread_cond_wait(&cook_cond, &tex);
  }
  order = 0;
  pthread_mutex_unlock(&tex);
}

void customer_controller(customer_id id) {
  pthread_mutex_lock(&tex);

  while (!food_at_table)
    pthread_cond_wait(&cust_cond, &tex);

  eat(id);
  food_at_table = 0;
  pthread_cond_signal(&waiter_customer_cond);

  // wait to see if we have eaten our fill
  pthread_cond_wait(&cust_cond, &tex);
  if (all_food_delivered) {
    customers_left = customers_left + 1;
    leave(id);
  }

  if (customers_left == NUM_CUSTOMERS) {
    pthread_cond_signal(&cook_cond);
  }

  pthread_mutex_unlock(&tex);
}
