#include "macros.h"
#include "vec_test.h"
#include "state_test.h"

// NOTE: 
// these tests leak memory big time, but this is not intended to be a long running
// application so it's honestly not worth the effort

int main() {
  TEST_GROUP(vector_test_all);
  TEST_GROUP(state_test_all);
}
