#include "state_test.h"
#include "macros.h"
#include "state_tests/state_tests.h"
#include <stdatomic.h>
extern state *GLOBAL_STATE;
extern FILE *LOG_FILE;

int WOULD_FAIL;

void inconsistent_state() { WOULD_FAIL = 1; }
void reset() { WOULD_FAIL = 0; }

void reset_state() {
  // if (LOG_FILE != NULL) {
  //   fclose(LOG_FILE);
  //   LOG_FILE = NULL;
  // }
  GLOBAL_STATE = init_state(
      GLOBAL_STATE->num_customers,
      GLOBAL_STATE->tables->len(GLOBAL_STATE->tables),
      GLOBAL_STATE->waitstaff_states->len(GLOBAL_STATE->waitstaff_states),
      GLOBAL_STATE->kitchen_state.num_cooks);
}

void state_test_all() {
  TEST_SUBGROUP(customer_state_test_all);
  reset_state();
  TEST_SUBGROUP(global_state_test_all);
}
