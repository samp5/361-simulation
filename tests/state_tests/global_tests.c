#include "../../simulator/sim.h"
#include "../macros.h"
#include "state_tests.h"
static int test_get_wait();

extern state *GLOBAL_STATE;

void global_state_test_all() { TEST(test_get_wait); }

static int test_get_wait() {
  int **arr;
  size_t len;
  get_waiting_line(arr, &len);

  FASSERT(len == GLOBAL_STATE->waitstaff_states->size,
          "The size of the returned waiting line should have the same size as "
          "the queue: Got %zu, expected %zu",
          len, GLOBAL_STATE->waitstaff_states->size);

  return 0;
}
