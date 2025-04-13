#include "../../sim.h"
#include "../log_macros.h"
#include "sync.h"
#include "util.h"
#include <unistd.h>

extern state *GLOBAL_STATE;
extern int MAX_BORSHT_ORDER;

/*
 * Prepare some borsht
 *
 * When this function returns, the requested borsht is ready to go out!
 * (Waitstaff can pick up this borsht order any time after this function
 * returns)
 *
 * params:
 *  `borsht_type` - type of borsht
 *  `quantitiy` - number of bowls to prepare
 */
void prepare(int borsht_type, int quantitiy) {
  int locks = Global;
  take(locks);
  LOG("The kitchen is starting to prepare %d bowls borsht type %d", quantitiy,
      borsht_type);

  if (borsht_type < 0 || borsht_type >= NUM_BORSHT_TYPE) {
    BAIL_AND_RELEASE(
        "Invalid Borsht type %d, expexected to between %d and %d (inclusive)",
        borsht_type, 0, NUM_BORSHT_TYPE - 1);
  }

  if (GLOBAL_STATE->bowls_prepared[borsht_type] + quantitiy >
      GLOBAL_STATE->bowls_ordered[borsht_type]) {
    BAIL_AND_RELEASE(
        "Only %d bowls of borsht type %d have been ordered, but preparing %d "
        "more bowls will mean the kitchen would have made %d bowls of this "
        "type. Be vigilant about borsht waste",
        GLOBAL_STATE->bowls_ordered[borsht_type], borsht_type, quantitiy,
        GLOBAL_STATE->bowls_prepared[borsht_type] + quantitiy);
  }

  if (quantitiy < 0) {
    BAIL_AND_RELEASE("Borsht quantitiy should not be less than zero, got: %d",
                     quantitiy);
  }

  // delay for some time
  usleep(1000 + 100 * quantitiy);

  // then increment the prepared bowls
  GLOBAL_STATE->kitchen_state.prepared_bowls[borsht_type] += quantitiy;
  GLOBAL_STATE->bowls_prepared[borsht_type] += quantitiy;

  release(locks);
}
