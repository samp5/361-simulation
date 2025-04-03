#include "macros.h"
#include "state_test.h"
#include "state_tests/init_state_test.h"
#include "state_tests/state_tests.h"
#include "vec_test.h"
#include <stdio.h>

// NOTE:
// these tests leak memory big time, but this is not intended to be a long
// running application so it's honestly not worth the effort

state *GLOBAL_STATE;
int TEST_COUNT = 0;
int FAILED_COUNT = 0;

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
  test_state_with(0, 100, 10, 0);
  test_state_with(0, 12, 10, 0);
  test_state_with(1, 13, 22, 14);
  test_state_with(6, 11, 123, 54);
  test_state_with(100, 100, 100, 100);

  if (!FAILED_COUNT) {
    printf(GREEN BOLD "\n-------------ALL %d TESTS PASSED-------------\n" RESET,
           TEST_COUNT);
  } else {
    printf(RED BOLD "\n-------------%d/%d TESTS FAILED-------------\n" RESET,
           FAILED_COUNT, TEST_COUNT);
  }
}
