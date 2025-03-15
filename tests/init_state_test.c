#include "init_state_test.h"
#include "macros.h"

static char str[1024];
static int test_state_empty();
static int test_waitstaff_empty();
static int test_waitstaff_single();
static int test_waitstaff_many();
static int test_waitstaff_many_unbalanced();

void init_state_test_all(){
  TEST(test_state_empty);
  TEST(test_waitstaff_empty);
  TEST(test_waitstaff_single);
  TEST(test_waitstaff_many);
  TEST(test_waitstaff_many_unbalanced);
}

static int test_state_empty(){
  state* s = init_state(0, 0, 0, 0);

  // customer state
  ASSERT(s->num_customers == 0, "Number of customers should be zero");
  ASSERT(s->num_customers_arrived == 0, "Initial state should have no cusomters arrived");
  ASSERT(s->seating_line->len(s->seating_line) == 0, "Seating line should be empty");


  // waitstaff
  ASSERT(s->waitstaff_states->len(s->waitstaff_states) == 0, "waitstaff_states should be empty");

  // table ids
  ASSERT(s->tables->len(s->tables) == 0, "tables should be empty");

  // kitchen
  ASSERT(s->kitchen_state.orders->len(s->kitchen_state.orders) == 0, "there should be no orders");

  for (int i = 0; i < BORSHT_TYPES; i++){
    ASSERT(s->kitchen_state.prepared_bowls[i] == 0, "all prepared BORSHT_TYPES should be 0");
  }
  return 0;
}

static int test_waitstaff_empty(){
  vector* ws = init_waitstaff_states(0, 0);
  ASSERT(ws->len(ws) == 0, "waitstaff should be length zero");
  return 0;
}

static int test_waitstaff_single(){
  vector* ws = init_waitstaff_states(1, 1);
  ASSERT(ws->len(ws) == 1, "waitstaff should be length 1");
  waitstaff w1;
  ASSERT(ws->get_at(ws, 0, &w1) == 0, "vector should contain element at index 0");
  ASSERT(w1.id == 0, "first waiter should have id 0");
  ASSERT(w1.table_ids->len(w1.table_ids) == 1, "waiter should have 1 table");
  for (int i = 0; i < BORSHT_TYPES; i++){
    ASSERT(w1.carrying[i] == 0, "initial carrying of all BORSHT_TYPES should be 0");
  }
  return 0;
}

static int test_waitstaff_many(){
  vector* ws = init_waitstaff_states(20, 100);
  ASSERT(ws->len(ws) == 20, "waitstaff should be length 20");
  table_id start = 0;
  for (int i = 0; i < 20; i++){
    waitstaff w_i;

    FASSERT(ws->get_at(ws, i, &w_i) == 0,"ws vector should have element at %d", i);
    FASSERT(w_i.id == i, "waiter should have id %d", i);
    ASSERT(w_i.table_ids->len(w_i.table_ids) == 5, "waiter should have 5 tables");

    for (int j = 0; j < 5; j++){

      table_id t_j;

      FASSERT(w_i.table_ids->get_at(w_i.table_ids, j, (void*)&t_j) == 0, "waiter %d should have table at index %d", i, j);
      FASSERT(t_j == start, "table should have id %d, got %d", start, t_j);

      start++;
    }

    for (int j = 0; j < BORSHT_TYPES; j++){
      ASSERT(w_i.carrying[j] == 0, "initial carrying of all BORSHT_TYPES should be 0");
    }
  }
  return 0;
}

static int test_waitstaff_many_unbalanced(){
  // the first waiter should have 6 tables.
  vector* ws = init_waitstaff_states(20, 101);
  ASSERT(ws->len(ws) == 20, "waitstaff should be length 20");


  // test the unbalanced waiter
  table_id start = 0;
  waitstaff w_i;
  FASSERT(ws->get_at(ws, 0, &w_i) == 0,"ws vector should have element at %d", 0);
  FASSERT(w_i.id == 0, "waiter should have id %d", 0);
  ASSERT(w_i.table_ids->len(w_i.table_ids) == 6, "waiter should have 6 tables");

  for (int j = 0; j < 6; j++){

    table t_j;

    FASSERT(w_i.table_ids->get_at(w_i.table_ids, j, (void*)&t_j) == 0, "waiter %d should have table at index %d", 0, j);
    FASSERT(t_j.id == start, "table should have id %d, got %d", start, t_j.id);
    FASSERT(t_j.current_status == Clean, "table id %d should be Clean", start);

    start++;
  }

  // test the remaining waiters
  for (int i = 1; i < 20; i++){
    waitstaff w_i;

    FASSERT(ws->get_at(ws, i, &w_i) == 0,"ws vector should have element at %d", i);
    FASSERT(w_i.id == i, "waiter should have id %d", i);
    ASSERT(w_i.table_ids->len(w_i.table_ids) == 5, "waiter should have 5 tables");

    for (int j = 0; j < 5; j++){

      table_id t_j;

      FASSERT(w_i.table_ids->get_at(w_i.table_ids, j, (void*)&t_j) == 0, "waiter %d should have table at index %d", i, j);
      FASSERT(t_j == start, "table should have id %d, got %d", start, t_j);

      start++;
    }

    for (int j = 0; j < BORSHT_TYPES; j++){
      ASSERT(w_i.carrying[j] == 0, "initial carrying of all BORSHT_TYPES should be 0");
    }
  }
  return 0;
}
