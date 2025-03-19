#include "macros.h"
#include "state_test.h"
#include "state_tests/init_state_test.h"
#include "state_tests/state_tests.h"
#include "vec_test.h"

// NOTE:
// these tests leak memory big time, but this is not intended to be a long
// running application so it's honestly not worth the effort

state *GLOBAL_STATE;

void test_state_with(int customers, int tables, int waiters, int cooks) {
  GLOBAL_STATE = init_state(customers, tables, waiters, cooks);
  TEST_INFO("Global state has %d customers, %d tables, %d waiters, %d cooks",
            customers, tables, waiters, cooks);
  reset();
  TEST_GROUP(state_test_all);
}

int main() {
  TEST_GROUP(vector_test_all);
  TEST_GROUP(init_state_test_all);

  test_state_with(0, 0, 0, 0);
  test_state_with(10, 10, 10, 10);
  // dump_state(GLOBAL_STATE);
  test_state_with(100, 100, 100, 100);
}
