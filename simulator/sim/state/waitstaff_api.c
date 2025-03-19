#include "../../sim.h"
#include "../log_macros.h"
#include "sync.h"
#include "util.h"
extern state *GLOBAL_STATE;

/////////////////////////////////////////////////////////////////////
//
// Wait staff API
//

/*
 * Get the tables assigned to this member of wait staff
 *
 * return by assignment:
 *  `tables` - after this function call *arr will point to an
 *        array of size `length` filled with `table_id`
 *  length - will be assigned the length of arr
 */
void get_tables(waitstaff_id id, table_id **tables, size_t *length) {
  int locks = Global;
  take(locks);

  waitstaff *target;
  if ((target = get_mut_waitstaff(id)) == NULL) {
    BAIL_AND_RELEASE("Waitstaff %d was NULL", id);
  }

  vector *table_vec = target->table_ids;
  table_id *arr = malloc(sizeof(table_id) * table_vec->len(table_vec));

  for (int i = 0; i < table_vec->len(table_vec); i++) {
    table_vec->get_at(table_vec, i, arr + i);
  }
  *length = table_vec->len(table_vec);

  release(locks);
}

/*
 * Seat a customer at a table.
 *
 * params:
 *  `customer_id` - id of customer
 *  `table_id` - id of table
 *
 * Pitfalls:
 *  1. Customer is already seated
 *  2. Table is not in waitstaff section
 *  3. Table is occupied
 */
void seat(waitstaff_id waitstaff_id, customer_id customer_id,
          table_id table_id) {

  int locks = Global;
  take(locks);

  customer *c;
  if ((c = get_mut_customer(customer_id)) == NULL) {
    BAIL_AND_RELEASE("Customer %d was NULL", customer_id);
  }

  table *t;
  if ((t = get_mut_table(table_id)) == NULL) {
    BAIL_AND_RELEASE("Table %d was NULL", table_id);
  }

  if (c->current_status != InQueue) {
  }

  c->table_id = t->id;

  release(locks);
}

/*
 * Get the borsht preference of a customer
 *
 * params:
 *  `customer_id` - id of customer 🧠
 *
 * return:
 *  `borsht_type` representing the borsht type preferred by
 *  customer with `c_id`
 *
 */
borsht_type intuit_preference(customer_id c_id) {
  int locks = Global;
  take(locks);

  LOG("Intuiting preference of Customer %d", c_id);
  customer *c;

  if ((c = get_mut_customer(c_id)) == NULL) {
    BAIL_RET_RELEASE(-1, "Customer %d was NULL", c_id);
  }

  if (c->current_status & NotArrived) {
    BAIL_RET_RELEASE(
        -1, "Tried to intuit the preference of Customer %d who hasn't arrived!",
        c_id);
  }

  borsht_type pref = c->preference;
  release(locks);
  return pref;
}

/*
 * Take the order of a customer at a table
 *
 * params:
 *  `customer_id` - id of customer
 *
 * return by assignment:
 *  `quantitiy` - number of bowls requested by customer
 *
 */
void take_order(waitstaff_id waitstaff_id, customer_id customer_id,
                int *quantitiy);

/*
 * Pick up borsht from the kitchen
 *
 * params:
 *  `borsht_type` - type of borsht to pick up
 *  `quantitiy` - number of bowls
 *
 */
void pick_up_borsht(waitstaff_id id, borsht_type borsht_type, int quantitiy);

/*
 * Pick up borsht from the kitchen
 *
 * params:
 *  `table_id` - table to serve to
 *  `borsht_type` - type of borsht to serve
 */
void serve(waitstaff_id waitstaff_id, table_id table_id,
           borsht_type borsht_type);

/*
 * Clean up table
 *
 * params:
 *  `table_id` - table to clean
 */
void clean_table(waitstaff_id w_id, table_id table_id);
