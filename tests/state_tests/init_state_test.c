#include "init_state_test.h"
#include "../macros.h"

static char str[1024];
static int test_state_empty();
static int test_waitstaff_empty();
static int test_waitstaff_single();
static int test_waitstaff_many();
static int test_waitstaff_many_unbalanced();
static int test_customers_empty();
static int test_customers_single();
extern const int NUM_BORSHT_TYPE;

void init_state_test_all() {
  TEST(test_state_empty);

  // waitstaff
  TEST(test_waitstaff_empty);
  TEST(test_waitstaff_single);
  TEST(test_waitstaff_many);
  TEST(test_waitstaff_many_unbalanced);

  // customers
  TEST(test_customers_empty);
  TEST(test_customers_single);
}

static int test_state_empty() {
  state *s = init_state(0, 0, 0, 0);

  // customer state
  ASSERT(s->num_customers == 0, "Number of customers should be zero");
  ASSERT(s->num_customers_arrived == 0,
         "Initial state should have no cusomters arrived");
  ASSERT(s->seating_line->len(s->seating_line) == 0,
         "Seating line should be empty");

  // waitstaff
  ASSERT(s->waitstaff_states->len(s->waitstaff_states) == 0,
         "waitstaff_states should be empty");

  // table ids
  ASSERT(s->tables->len(s->tables) == 0, "tables should be empty");

  // kitchen
  for (int i = 0; i < NUM_BORSHT_TYPE; i++) {
    ASSERT(s->kitchen_state.prepared_bowls[i] == 0,
           "all prepared BORSHT_TYPES should be 0");
  }
  return 0;
}

static int test_waitstaff_empty() {
  vector *ws = init_waitstaff_states(0, 0);
  ASSERT(ws->len(ws) == 0, "waitstaff should be length zero");
  return 0;
}

static int test_waitstaff_single() {
  vector *ws = init_waitstaff_states(1, 1);
  ASSERT(ws->len(ws) == 1, "waitstaff should be length 1");
  waitstaff w1;
  ASSERT(ws->get_at(ws, 0, &w1) == 0,
         "vector should contain element at index 0");
  ASSERT(w1.id == 0, "first waiter should have id 0");
  ASSERT(w1.table_ids->len(w1.table_ids) == 1, "waiter should have 1 table");
  for (int i = 0; i < NUM_BORSHT_TYPE; i++) {
    ASSERT(w1.carrying[i] == 0,
           "initial carrying of all BORSHT_TYPES should be 0");
  }
  return 0;
}

static int test_waitstaff_many() {
  vector *ws = init_waitstaff_states(20, 100);
  ASSERT(ws->len(ws) == 20, "waitstaff should be length 20");
  table_id start = 0;
  for (int i = 0; i < 20; i++) {
    waitstaff w_i;

    FASSERT(ws->get_at(ws, i, &w_i) == 0, "ws vector should have element at %d",
            i);
    FASSERT(w_i.id == i, "waiter should have id %d", i);
    ASSERT(w_i.table_ids->len(w_i.table_ids) == 5,
           "waiter should have 5 tables");

    for (int j = 0; j < 5; j++) {

      table_id t_j;

      FASSERT(w_i.table_ids->get_at(w_i.table_ids, j, (void *)&t_j) == 0,
              "waiter %d should have table at index %d", i, j);
      FASSERT(t_j == start, "table should have id %d, got %d", start, t_j);

      start++;
    }

    for (int j = 0; j < NUM_BORSHT_TYPE; j++) {
      ASSERT(w_i.carrying[j] == 0,
             "initial carrying of all BORSHT_TYPES should be 0");
    }
  }
  return 0;
}

static int test_waitstaff_many_unbalanced() {
  // the first waiter should have 6 tables.
  vector *ws = init_waitstaff_states(20, 101);
  ASSERT(ws->len(ws) == 20, "waitstaff should be length 20");

  // test the unbalanced waiter
  table_id start = 0;
  waitstaff w_i;
  FASSERT(ws->get_at(ws, 0, &w_i) == 0, "ws vector should have element at %d",
          0);
  FASSERT(w_i.id == 0, "waiter should have id %d", 0);
  ASSERT(w_i.table_ids->len(w_i.table_ids) == 6, "waiter should have 6 tables");

  for (int j = 0; j < 6; j++) {

    table_id t_j;

    FASSERT(w_i.table_ids->get_at(w_i.table_ids, j, (void *)&t_j) == 0,
            "waiter %d should have table at index %d", 0, j);
    FASSERT(t_j == start, "table should have id %d, got %d", start, t_j);

    start++;
  }

  // test the remaining waiters
  for (int i = 1; i < 20; i++) {
    waitstaff w_i;

    FASSERT(ws->get_at(ws, i, &w_i) == 0, "ws vector should have element at %d",
            i);
    FASSERT(w_i.id == i, "waiter should have id %d", i);
    ASSERT(w_i.table_ids->len(w_i.table_ids) == 5,
           "waiter should have 5 tables");

    for (int j = 0; j < 5; j++) {

      table_id t_j;

      FASSERT(w_i.table_ids->get_at(w_i.table_ids, j, (void *)&t_j) == 0,
              "waiter %d should have table at index %d", i, j);
      FASSERT(t_j == start, "table should have id %d, got %d", start, t_j);

      start++;
    }

    for (int j = 0; j < NUM_BORSHT_TYPE; j++) {
      ASSERT(w_i.carrying[j] == 0,
             "initial carrying of all BORSHT_TYPES should be 0");
    }
  }
  return 0;
}

static int test_customers_empty() {
  vector *customers = init_customers(0);
  ASSERT(customers->len(customers) == 0, "Customers should be empty");
  return 0;
}

static int test_customers_single() {
  vector *customers = init_customers(1);
  ASSERT(customers->len(customers) == 1, "Customers should have 1 element");

  customer c;
  customers->get_at(customers, 0, (void *)&c);
  ASSERT(c.table_id == -1, "Customer should not be assigned a table");
  ASSERT(c.borsht_eaten == 0, "Customer should not be assigned a table");
  ASSERT(c.borsht_desired > 0,
         "Customer should want at least one bowl of borst");
  ASSERT(c.current_status == NotArrived,
         "Customer should only have status NotArrived");
  ASSERT(c.preference >= 0 && c.preference < NUM_BORSHT_TYPE,
         "Customer should have a valid borst preference");

  customer dummy;
  dummy.id = 0;
  ASSERT(customers->find(customers, (void *)&dummy) == 0,
         "Customer cannot be found in vector");

  return 0;
}
