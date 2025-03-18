#include "../macros.h"
#include "state_tests.h"
#include <unistd.h>

extern state *GLOBAL_STATE;
static int test_has_arrived();
static int test_nonexistent_customer();
static int test_basic();
static int test_basic_fail();
static int test_basic_fail2();

void reset_state() {
  GLOBAL_STATE = init_state(
      GLOBAL_STATE->num_customers,
      GLOBAL_STATE->tables->len(GLOBAL_STATE->tables),
      GLOBAL_STATE->waitstaff_states->len(GLOBAL_STATE->waitstaff_states),
      GLOBAL_STATE->kitchen_state.num_cooks);
}

void customer_state_test_all() {
  TEST(test_has_arrived);
  reset_state();
  TEST(test_nonexistent_customer);
  reset_state();
  TEST(test_basic);
  reset_state();
  TEST(test_basic_fail);
  reset_state();
  TEST(test_basic_fail2);
  reset_state();
}

static int test_has_arrived() {
  int expected_status;

#ifdef STAGGERED_ARRIVE
  expected_status = 0;
#else
  expected_status = 1;
#endif

  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {
    int arrived = has_arrived(i);
    FASSERT(arrived == expected_status, "Expected customer %d to be NotArrived",
            i);
  }
  return 0;
}

static int test_nonexistent_customer() {
  int expected_status;

#ifdef STAGGERED_ARRIVE
  expected_status = 0;
#else
  expected_status = 1;
#endif

  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {
    int arrived = has_arrived(i);
    FASSERT(arrived == expected_status, "Expected customer %d to be NotArrived",
            i);
  }
  int nonexistent = GLOBAL_STATE->num_customers;
  ASSERT_FAIL("Customer id should not exist", has_arrived, nonexistent);
  return 0;
}

static void assign_customers_to_tables() {
  int table_index = 0, customer_index = 0;
  vector *customers = GLOBAL_STATE->customers;
  vector *tables = GLOBAL_STATE->tables;

  // Assign each customer to one table
  while (table_index < GLOBAL_STATE->tables->size &&
         customer_index < GLOBAL_STATE->num_customers) {

    customer *c_i;
    customers->get_mut_at(customers, customer_index, (void **)&c_i);
    c_i->current_status = AtTable | Ordered;

    table *t_i;
    tables->get_mut_at(tables, table_index, (void **)&t_i);
    t_i->current_status = Occupied;

    c_i->table_id = t_i->id;

    table_index++;
    customer_index++;
  }
}

static int test_basic() {
  if (GLOBAL_STATE->num_customers == 0 || GLOBAL_STATE->tables->len == 0) {
    return 0;
  }

  vector *customers = GLOBAL_STATE->customers;
  vector *tables = GLOBAL_STATE->tables;
  assign_customers_to_tables();

  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {

    // get customer
    customer *c_i;
    customers->get_mut_at(customers, i, (void **)&c_i);

    // find corresponding table
    table *t_i;
    table dummy;

    dummy.id = c_i->id;
    int table_index = tables->find(tables, (void *)&dummy);
    tables->get_mut_at(tables, table_index, (void **)&t_i);

    // give each customer a bowl of borsht
    t_i->borsht_bowls[c_i->preference] = 1;

    ASSERT_VALID("Customer should be able to eat", eat, c_i->id);
  }

  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {
    // get customer
    customer *c_i;
    customers->get_mut_at(customers, i, (void **)&c_i);

    // find corresponding table
    table *t_i;
    table dummy;

    dummy.id = c_i->table_id;

    int table_index = tables->find(tables, (void *)&dummy);
    tables->get_mut_at(tables, table_index, (void **)&t_i);

    ASSERT(t_i->borsht_bowls[c_i->preference] == 0,
           "Customer should have eaten that borsht!");
    ASSERT(c_i->current_status == (AtTable | Ordered),
           "Customer should have status AtTable | Ordered");
    ASSERT(c_i->borsht_eaten == 1,
           "Customer should have 1 bowl of borsht eaten");
  }

  return 0;
}

static int test_basic_fail() {
  if (GLOBAL_STATE->num_customers == 0 || GLOBAL_STATE->tables->len == 0) {
    return 0;
  }

  vector *customers = GLOBAL_STATE->customers;
  vector *tables = GLOBAL_STATE->tables;
  assign_customers_to_tables();

  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {

    // get customer
    customer *c_i;
    customers->get_mut_at(customers, i, (void **)&c_i);

    // find corresponding table
    table *t_i;
    table dummy;

    dummy.id = c_i->id;
    int table_index = tables->find(tables, (void *)&dummy);
    tables->get_mut_at(tables, table_index, (void **)&t_i);

    // give each customer a bowl of borsht
    t_i->borsht_bowls[c_i->preference] = 0;

    ASSERT_FAIL("Customer should not be able to eat", eat, c_i->id);
  }

  return 0;
}

static int test_basic_fail2() {
  if (GLOBAL_STATE->num_customers == 0 || GLOBAL_STATE->tables->len == 0) {
    return 0;
  }

  vector *customers = GLOBAL_STATE->customers;
  vector *tables = GLOBAL_STATE->tables;
  assign_customers_to_tables();

  for (int i = 0; i < GLOBAL_STATE->num_customers; i++) {

    // get customer
    customer *c_i;
    customers->get_mut_at(customers, i, (void **)&c_i);
    c_i->current_status = Eating | AtTable;

    // find corresponding table
    table *t_i;
    table dummy;

    dummy.id = c_i->id;
    int table_index = tables->find(tables, (void *)&dummy);
    tables->get_mut_at(tables, table_index, (void **)&t_i);

    // give each customer a bowl of borsht
    t_i->borsht_bowls[c_i->preference] = 1;

    ASSERT_FAIL("Customer should not be able to eat", eat, c_i->id);
  }

  return 0;
}
