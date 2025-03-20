#include "../macros.h"
#include "state_tests.h"

extern state *GLOBAL_STATE;

// get_tables
static int test_get_tables();
static int test_get_tables_non_existent();

// seat
static int test_seat();
static int test_seat_waitstaff_nonexistent();
static int test_seat_customer_not_in_line();
static int test_seat_customer_not_in_front();
static int test_seat_table_occupied();
static int test_seat_table_dirty();

// intuit_preference

// take_order

// pick_up_borsht

// serve

// clean_table
void waitstaff_state_test_all() {
  TEST(test_get_tables);
  reset_state();
  TEST(test_get_tables_non_existent);
  reset_state();
  TEST(test_seat);
  reset_state();
  TEST(test_seat_waitstaff_nonexistent);
  reset_state();
  TEST(test_seat_customer_not_in_line);
  reset_state();
  TEST(test_seat_customer_not_in_front);
  reset_state();
  TEST(test_seat_table_occupied);
  reset_state();
  TEST(test_seat_table_dirty);
  reset_state();
}

static int test_get_tables() {
  vector *ws = GLOBAL_STATE->waitstaff_states;
  if (ws->len(ws) == 0) {
    return 0;
  }

  for (int i = 0; i < ws->len(ws); i++) {
    table_id *arr;
    size_t arr_len;

    waitstaff w_i;
    FASSERT(ws->get_at(ws, i, (void *)&w_i) == 0,
            "Waitstaff at %d should exist", i);

    get_tables(w_i.id, &arr, &arr_len);

    for (size_t j = 0; j < arr_len; j++) {
      FASSERT(w_i.table_ids->find(w_i.table_ids, (void *)(arr + j)) != -1,
              "Table id %d should be in Waiterstaff %d's table vector", arr[j],
              w_i.id);
    }
  }

  return 0;
}
static int test_get_tables_non_existent() {
  vector *ws = GLOBAL_STATE->waitstaff_states;
  if (ws->len(ws) == 0) {
    return 0;
  }

  ASSERT_FAIL("Non existent waiter ID should cause inconsistent state",
              get_tables, ws->len(ws), NULL, NULL);

  return 0;
}

// seating tests
static int test_seat() { return 0; }
static int test_seat_waitstaff_nonexistent() { return 0; }
static int test_seat_customer_not_in_line() { return 0; }
static int test_seat_customer_not_in_front() { return 0; }
static int test_seat_table_occupied() { return 0; }
static int test_seat_table_dirty() { return 0; }
