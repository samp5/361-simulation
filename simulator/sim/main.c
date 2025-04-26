#include "../restaurant.h"
#include "controllers.h"
#include "state/state.h"
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

void inconsistent_state() { exit(1); }
state *GLOBAL_STATE;

pthread_t customers[NUM_CUSTOMERS];
pthread_t waiters[NUM_WAITERS];
pthread_t cooks[NUM_COOKS];

pthread_barrier_t customer_barrier;
pthread_barrier_t cook_barrier;
pthread_barrier_t waitstaff_barrier;

void *start_customer(void *id) {
  pthread_barrier_wait(&customer_barrier);

  while (!GLOBAL_STATE->is_done(GLOBAL_STATE)) {
    customer_controller((size_t)id);
  }

  pthread_exit(NULL);
}

void *start_cook() {
  pthread_barrier_wait(&cook_barrier);

  while (!GLOBAL_STATE->is_done(GLOBAL_STATE)) {
    cook_controller();
  }
  pthread_exit(NULL);
}

void *start_waitstaff(void *id) {
  pthread_barrier_wait(&waitstaff_barrier);

  while (!GLOBAL_STATE->is_done(GLOBAL_STATE)) {
    wait_staff_controller((size_t)id);
  }
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  initializer();

  GLOBAL_STATE = init_state(NUM_CUSTOMERS, NUM_TABLES, NUM_WAITERS, NUM_COOKS);
  pthread_barrier_init(&customer_barrier, NULL, NUM_CUSTOMERS + 1);
  pthread_barrier_init(&waitstaff_barrier, NULL, NUM_WAITERS + 1);
  pthread_barrier_init(&cook_barrier, NULL, NUM_COOKS + 1);

  // Create all threads
  for (size_t i = 0; i < NUM_CUSTOMERS; i++) {
    pthread_create(customers + i, NULL, start_customer, (void *)i);
  }
  for (size_t i = 0; i < NUM_WAITERS; i++) {
    pthread_create(waiters + i, NULL, start_waitstaff, (void *)i);
  }
  for (size_t i = 0; i < NUM_COOKS; i++) {
    pthread_create(cooks + i, NULL, start_cook, (void *)i);
  }

  // signal that they can start
  pthread_barrier_wait(&customer_barrier);
  pthread_barrier_wait(&waitstaff_barrier);
  pthread_barrier_wait(&cook_barrier);

  // Join all threads
  for (int i = 0; i < NUM_CUSTOMERS; i++) {
    pthread_join(customers[i], NULL);
  }
  for (int i = 0; i < NUM_WAITERS; i++) {
    pthread_join(waiters[i], NULL);
  }
  for (int i = 0; i < NUM_COOKS; i++) {
    pthread_join(cooks[i], NULL);
  }
}
