#include "../macros.h"
#include "state_tests.h"
#include "utils/utils.h"
#include <stdio.h>

extern state *GLOBAL_STATE;

// get_tables
static int test_get_tables();
static int test_get_tables_non_existent();

// seat
static int test_seat_one();
static int test_seat();
static int test_seat_waitstaff_nonexistent();
static int test_seat_table_nonexistent();
static int test_seat_customer_nonexistent();
static int test_seat_customer_not_in_line();
static int test_seat_customer_not_in_front();
static int test_seat_table_occupied();
static int test_seat_table_dirty();

// intuit_preference
static int test_intuit_pref();
static int test_intuit_pref_nonexistent_customer();

// take_order
static int test_take_order();
static int test_take_order_not_seated();
static int test_take_order_not_in_section();
static int test_take_order_already_ordered();
static int test_take_order_nonexistent_customer();
static int test_take_order_nonexistent_waitstaff();

// pick_up_borsht
// TODO:

// serve
// TODO:

// clean_table
// TODO:

void waitstaff_state_test_all() {
  // get tables
  TEST(test_get_tables);
  reset_state();
  TEST(test_get_tables_non_existent);
  reset_state();

  // seat
  TEST(test_seat_one);
  reset_state();
  TEST(test_seat);
  reset_state();
  TEST(test_seat_waitstaff_nonexistent);
  reset_state();
  TEST(test_seat_table_nonexistent);
  reset_state();
  TEST(test_seat_customer_nonexistent);
  reset_state();
  TEST(test_seat_customer_not_in_line);
  reset_state();
  TEST(test_seat_customer_not_in_front);
  reset_state();
  TEST(test_seat_table_occupied);
  reset_state();
  TEST(test_seat_table_dirty);
  reset_state();

  // intuit_preference
  TEST(test_intuit_pref);
  reset_state();
  TEST(test_intuit_pref_nonexistent_customer);
  reset_state();

  // take order
  TEST(test_take_order);
  reset_state();
  TEST(test_take_order_not_seated);
  reset_state();
  TEST(test_take_order_not_in_section);
  reset_state();
  TEST(test_take_order_already_ordered);
  reset_state();
  TEST(test_take_order_nonexistent_customer);
  reset_state();
  TEST(test_take_order_nonexistent_waitstaff);
  reset_state();
}

///////////////////////////////////////////////////////////////////////////////
//
//
//   GET TABLE TESTS
//
//
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

  ASSERT_FAIL("Nonexistent waiter ID should cause inconsistent state",
              get_tables, ws->len(ws), NULL, NULL);

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//
//    SEATING TESTS
//
//
static int test_seat_one() {
  vector *ws = GLOBAL_STATE->waitstaff_states;
  if (ws->len(ws) == 0)
    return 0;

  queue *line = GLOBAL_STATE->seating_line;
  if (line->len(line) == 0)
    return 0;

  waitstaff *w = NULL;
  // find the first valid waitstaff member with a table
  for (int i = 0; i < ws->len(ws); i++) {
    ws->get_mut_at(ws, i, (void **)&w);
    if (w->table_ids->len(w->table_ids) > 0)
      break;
  }

  // there are no waitstaff with tables, this is still technically a valid state
  if (w == NULL) {
    return 0;
  }

  // we have at least one waiter and one customer

  // get the first customer in line
  customer_id first;
  line->peek(line, &first);

  // get the first table
  table_id t_id;
  w->table_ids->get_at(w->table_ids, 0, (void *)&t_id);

  // we should be able to seat them
  ASSERT_VALID(
      "Waitstaff should be able to seat the customer at the front of the line",
      seat, w->id, first, t_id);

  table *t = get_table(t_id);
  FASSERT(t->current_status == Occupied,
          "Table %d was just seated and so should be occupied", t_id);
  FASSERT(t->customer_id == first,
          "Table %d was just seated and should have customer_id %d", t_id,
          first);

  customer *c = get_customer(first);
  FASSERT(c->current_status == AtTable,
          "Customer %d was just seated and so should have status AtTable",
          c->id);
  FASSERT(c->table_id == t_id,
          "Customer %d was just seated at Table %d but has Table %d in struct",
          c->id, t_id, c->table_id);
  return 0;
}

static int test_seat() {
  vector *ws = GLOBAL_STATE->waitstaff_states;
  if (ws->len(ws) == 0)
    return 0;

  // while there are customers in line
  queue *line = GLOBAL_STATE->seating_line;
  customer_id first;
  while (line->peek(line, &first) != -1) {

    // find the first valid waitstaff member with a table
    waitstaff *w = NULL;
    table *t = NULL;

    int res = get_first_available_table(&t, &w);

    if (res == -1) {
      return 0;
    } else {
      // we should be able to seat them
      ASSERT_VALID("Waitstaff should be able to seat the customer at the front "
                   "of the line",
                   seat, w->id, first, t->id);

      FASSERT(t->current_status == Occupied,
              "Table %d was just seated and so should be occupied", t->id);
      FASSERT(t->customer_id == first,
              "Table %d was just seated and should have customer_id %d", t->id,
              first);

      customer *c = get_customer(first);
      FASSERT(c->current_status == AtTable,
              "Customer %d was just seated and so should have status AtTable",
              c->id);
    }
  }

  return 0;
}

static int test_seat_waitstaff_nonexistent() {
  vector *ws = GLOBAL_STATE->waitstaff_states;
  table *t;
  waitstaff *w;

  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }

  // get first customer
  customer_id first;
  queue *q = GLOBAL_STATE->seating_line;
  if (q->peek(q, &first) != 0) {
    return 0;
  }

  ASSERT_FAIL(
      "Should not be able to have a nonexistent waitstaff seat a customer",
      seat, ws->len(ws), first, t->id);

  return 0;
}

static int test_seat_table_nonexistent() {
  vector *ws = GLOBAL_STATE->waitstaff_states;
  table *t;
  waitstaff *w;

  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }

  // get first customer
  customer_id first;
  queue *q = GLOBAL_STATE->seating_line;
  if (q->peek(q, &first) != 0) {
    return 0;
  }

  table_id nonexistent_table = GLOBAL_STATE->tables->len(GLOBAL_STATE->tables);
  ASSERT_FAIL("Should not be able to seat a customer at a nonexistent table",
              seat, w->id, first, nonexistent_table);

  return 0;
}

static int test_seat_customer_nonexistent() {
  vector *ws = GLOBAL_STATE->waitstaff_states;
  table *t;
  waitstaff *w;

  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }

  // get first customer
  customer_id nonexistent = GLOBAL_STATE->num_customers;
  ASSERT_FAIL("Should not be able to seat a customer at a nonexistent table",
              seat, w->id, nonexistent, t->id);

  return 0;
}

static int test_seat_customer_not_in_line() {
  queue *q = GLOBAL_STATE->seating_line;
  if (q->len(q) < 1) {
    return 0;
  }

  table *t;
  waitstaff *w;
  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }

  // get first customer
  customer_id first;
  if (q->peek(q, &first) != 0) {
    return 0;
  }

  ASSERT_VALID("Should be able to seat a customer at the front of line", seat,
               w->id, first, t->id);

  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }

  ASSERT_FAIL("Should not be able to seat a customer who is not in line", seat,
              w->id, first, t->id);

  return 0;
}

static int test_seat_customer_not_in_front() {
  queue *q = GLOBAL_STATE->seating_line;
  if (q->len(q) < 2) {
    return 0;
  }

  table *t;
  waitstaff *w;
  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }

  customer_id not_first;
  q->get_at(q, q->len(q) - 1, &not_first);

  ASSERT_FAIL("Should not be able to seat a customer at the back of the line",
              seat, w->id, not_first, t->id);

  return 0;
}

static int test_seat_table_occupied() {
  queue *q = GLOBAL_STATE->seating_line;
  if (q->len(q) < 2) {
    return 0;
  }

  table *t;
  waitstaff *w;
  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }

  // get first customer
  customer_id first;
  if (q->peek(q, &first) != 0) {
    return 0;
  }

  ASSERT_VALID("Should be able to seat a customer at the front of line", seat,
               w->id, first, t->id);

  // get the next customer
  if (q->peek(q, &first) != 0) {
    return 0;
  }
  ASSERT_FAIL("Should not be able to seat customer at occupied table", seat,
              w->id, first, t->id);

  return 0;
}

static int test_seat_table_dirty() {
  queue *q = GLOBAL_STATE->seating_line;
  if (q->len(q) < 2) {
    return 0;
  }

  table *t;
  waitstaff *w;
  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }

  t->current_status = Dirty;

  // get first customer
  customer_id first;
  if (q->peek(q, &first) != 0) {
    return 0;
  }

  ASSERT_FAIL("Should not be able to seat customer at dirty table", seat, w->id,
              first, t->id);

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//
//   INTUIT PREFERENCE TESTS
//
//
static int test_intuit_pref() {
  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {
    customer *c = get_customer(i);
    borsht_type bt = intuit_preference(i);
    FASSERT(c->preference == bt, "Expected %d == %d, got %d != %d",
            c->preference, c->preference, c->preference, bt);
  }
  return 0;
}

static int test_intuit_pref_nonexistent_customer() {
  ASSERT_FAIL("Expected failure on nonexistent customer", intuit_preference,
              GLOBAL_STATE->num_customers);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//
//   TAKE ORDER TESTS
//
//
static int test_take_order() {
  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {
    table *t;
    waitstaff *w;
    if (get_first_available_table(&t, &w) != 0) {
      return 0;
    }
    customer *c = get_customer(i);
    ASSERT_VALID("Should be able to seat a customer", seat, w->id, c->id,
                 t->id);

    int order;
    ASSERT_VALID("Should be able to take an order of seated customer",
                 take_order, w->id, c->id, &order);
    FASSERT(order == c->borsht_desired,
            "Customer %d wanted %d but take order gave %d", c->id,
            c->borsht_desired, order);
  }
  return 0;
}

static int test_take_order_not_seated() {
  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {
    table *t;
    waitstaff *w;
    if (get_first_available_table(&t, &w) != 0) {
      return 0;
    }
    customer *c = get_customer(i);
    int order;
    ASSERT_FAIL(
        "Should not be able to take an order of customer who is not seated",
        take_order, w->id, c->id, &order);
  }
  return 0;
}

static int test_take_order_not_in_section() {
  vector *ws = GLOBAL_STATE->waitstaff_states;

  // we need at least two waitstaff for this test to make sense
  if (ws->len(ws) < 2) {
    return 0;
  }

  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {
    table *t;
    waitstaff *w;
    if (get_first_available_table(&t, &w) != 0) {
      return 0;
    }
    customer *c = get_customer(i);
    ASSERT_VALID("Should be able to seat a customer", seat, w->id, c->id,
                 t->id);

    waitstaff_id other_w = (w->id + 1) % ws->len(ws);
    int order;
    ASSERT_FAIL(
        "Customer not in proper section, should not be able to take order",
        take_order, other_w, c->id, &order);
  }
  return 0;
}

static int test_take_order_already_ordered() {

  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {
    table *t;
    waitstaff *w;
    if (get_first_available_table(&t, &w) != 0) {
      return 0;
    }
    customer *c = get_customer(i);
    ASSERT_VALID("Should be able to seat a customer", seat, w->id, c->id,
                 t->id);

    int order;
    ASSERT_VALID("Should be able to take an order of seated customer",
                 take_order, w->id, c->id, &order);
    ASSERT_FAIL(
        "Should NOT be able to take an order of customer who already ordered",
        take_order, w->id, c->id, &order);
  }
  return 0;
}

static int test_take_order_nonexistent_customer() {
  table *t;
  waitstaff *w;
  if (get_first_available_table(&t, &w) != 0) {
    return 0;
  }
  customer_id nonexistent = GLOBAL_STATE->num_customers;
  int order;
  ASSERT_FAIL("Nonexistent customer should fail", take_order, w->id,
              nonexistent, &order);
  return 0;
}

static int test_take_order_nonexistent_waitstaff() {
  if (GLOBAL_STATE->num_customers <= 0)
    return 0;

  int order;
  waitstaff_id nonexistent =
      GLOBAL_STATE->waitstaff_states->len(GLOBAL_STATE->waitstaff_states);

  ASSERT_FAIL("Nonexistent waitstaff should fail", take_order, nonexistent, 0,
              &order);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//
//   PICK UP BORSHT TESTS
//
//

///////////////////////////////////////////////////////////////////////////////
//
//
//   SERVE TESTS
//
//

///////////////////////////////////////////////////////////////////////////////
//
//
//   CLEAN TABLE TESTS
//
//
