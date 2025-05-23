#include "../../sim.h"
#include "../log_macros.h"
#include "state.h"
#include "sync.h"
#include <stdlib.h>

extern state *GLOBAL_STATE;
// Global API
void get_waiting_line(customer_id **arr, size_t *len) {
  int locks = Global;
  take(locks);

  queue *q = GLOBAL_STATE->seating_line;
  customer_id *q_arr = malloc(sizeof(customer_id) * q->len(q));
  for (int i = 0; i < q->len(q); i++) {
    q->get_at(q, i, q_arr + i);
  }

  *arr = q_arr;
  *len = q->len(q);
  release(locks);
}

int types_of_borsht() { return NUM_BORSHT_TYPE; }
