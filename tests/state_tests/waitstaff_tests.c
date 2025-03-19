#include "../macros.h"
#include "state_tests.h"

extern state *GLOBAL_STATE;
static int test_get_tables();
static int test_get_tables_non_existent();

void waitstaff_state_test_all() {
  TEST(test_get_tables);
  TEST(test_get_tables_non_existent);
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

    for (int j = 0; j < arr_len; j++) {
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
