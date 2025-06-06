#include "macros.h"
#include "state_tests/state_tests.h"
#include <stdatomic.h>
extern state *GLOBAL_STATE;
extern FILE *LOG_FILE;

int WOULD_FAIL;
int PASSED_ALL = 1;

void inconsistent_state() { WOULD_FAIL = 1; }
void reset() { WOULD_FAIL = 0; }

void reset_state() {
  if (LOG_FILE != NULL) {
    fclose(LOG_FILE);
    LOG_FILE = NULL;
  }

  state *new_state = init_state(
      GLOBAL_STATE->num_customers,
      GLOBAL_STATE->tables->len(GLOBAL_STATE->tables),
      GLOBAL_STATE->waitstaff_states->len(GLOBAL_STATE->waitstaff_states),
      GLOBAL_STATE->kitchen_state.num_cooks);

  dealloc_state(&GLOBAL_STATE);

  GLOBAL_STATE = new_state;
}

void state_test_all() {
  TEST_SUBGROUP(customer_state_test_all);
  reset_state();
  TEST_SUBGROUP(global_state_test_all);
  reset_state();
  TEST_SUBGROUP(waitstaff_state_test_all);
}
