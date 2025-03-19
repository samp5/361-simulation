#include "state_test.h"
#include "macros.h"
#include "state_tests/state_tests.h"
#include <stdatomic.h>
extern state *GLOBAL_STATE;
int WOULD_FAIL;

void inconsistent_state() { WOULD_FAIL = 1; }
void reset() { WOULD_FAIL = 0; }

void state_test_all() {
  TEST_SUBGROUP(customer_state_test_all);
  reset_state();
  TEST_SUBGROUP(global_state_test_all);
}
