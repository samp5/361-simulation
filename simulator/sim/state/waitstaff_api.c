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
  *tables = arr;

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
 *  1. Customer is not in line - x
 *  2. Customer is not in the front of the line - x
 *  3. Table is not in waitstaff section - x
 *  4. Table is occupied or dirty
 */
void seat(waitstaff_id w_id, customer_id c_id, table_id t_id) {

  int locks = Global;
  take(locks);
  LOG("Waitstaff %d is trying to seat Customer %d at Table %d", w_id, c_id,
      t_id);

  customer *c;
  if ((c = get_mut_customer(c_id)) == NULL) {
    BAIL_AND_RELEASE("Customer %d was NULL", c_id);
  }

  // 1. Customer is not in line
  if (c->current_status != InQueue) {
    BAIL_AND_RELEASE("Customer %d was not in line!", c->id);
  }

  //  2. Customer is not in the front of the line
  customer_id front;
  GLOBAL_STATE->seating_line->peek(GLOBAL_STATE->seating_line, &front);
  if (front != c_id) {
    BAIL_AND_RELEASE("Customer %d was in line but not in front!", c->id);
  }

  //  3. Table is not in waitstaff section
  waitstaff *w;
  if ((w = get_mut_waitstaff(w_id)) == NULL) {
    BAIL_AND_RELEASE("Waitstaff %d was NULL", c_id);
  }

  int found = w->table_ids->find(w->table_ids, (void *)&t_id);
  if (found == -1) {
    BAIL_AND_RELEASE("Table %d does not belong to waitstaff member %d!", t_id,
                     w_id);
  }

  //  4. Table is dirty or occupied
  table *t;
  if ((t = get_mut_table(t_id)) == NULL) {
    BAIL_AND_RELEASE("Table %d was NULL", t_id);
  }

  switch (t->current_status) {
  case Occupied:
    BAIL_AND_RELEASE("Table %d was is already occupied!", t_id);
    break;
  case Dirty:
    BAIL_AND_RELEASE("Table %d is Dirty!", t_id);
    break;
  case Clean:
    break;
  }

  // Everything is OK!
  // Remove customer from queue and update both their status
  // and the table they are sitting at.
  remove_customer_from_queue(c_id);
  c->table_id = t->id;
  c->current_status = AtTable;
  t->current_status = Occupied;

  LOG("Customer %d was just seated at Table %d by Waitstaff %d", c->id, t->id,
      w_id);

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
 * Pitfalls:
 *  1. Customer does not exist
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
 * Pitfalls:
 *  1. Customer does not exist
 *  2. Waitstaff does not exist
 *  3. Customer is not seated
 *  4. Customer already ordered
 *  5. Customer is not in waitstaffs section
 */
void take_order(waitstaff_id waitstaff_id, customer_id customer_id,
                int *quantitiy) {

  int locks = Global;
  take(locks);
  LOG("Waitstaff %d is trying to take the order of Customer %d", waitstaff_id,
      customer_id);

  // 1. Customer does not exist
  customer *c;
  if ((c = get_mut_customer(customer_id)) == NULL) {
    BAIL_AND_RELEASE("Customer %d was NULL", customer_id);
  }

  //  2. Waitstaff does not exist
  waitstaff *w;
  if ((w = get_mut_waitstaff(waitstaff_id)) == NULL) {
    BAIL_AND_RELEASE("Waitstaff %d was NULL", customer_id);
  }
  // 3. Customer is not seated
  if (!(c->current_status & AtTable)) {
    BAIL_AND_RELEASE("Customer %d has not been seated!", c->id);
  }

  // 4. Customer already ordered
  if (c->current_status & Ordered) {
    BAIL_AND_RELEASE("Customer %d already ordered!", c->id);
  }

  // 5. Table is not in this waitstaffs section
  int found = w->table_ids->find(w->table_ids, (void *)&c->table_id);
  if (found == -1) {
    BAIL_AND_RELEASE("Customer is at Table %d, but this table does not belong "
                     "to waitstaff member %d!",
                     c->table_id, w->id);
  }

  LOG("Waitstaff %d just took the order of Customer %d at Table %d, they want "
      "%d bowls!",
      w->id, c->id, c->table_id, c->borsht_desired);

  *quantitiy = c->borsht_desired;
  c->current_status = Ordered & AtTable;

  release(locks);
}

/*
 * Pick up borsht from the kitchen
 *
 * params:
 *  `borsht_type` - type of borsht to pick up
 *  `quantitiy` - number of bowls
 *
 */
void pick_up_borsht(waitstaff_id id, borsht_type borsht_type, int quantitiy) {
  // TODO:
}

/*
 * Pick up borsht from the kitchen
 *
 * params:
 *  `table_id` - table to serve to
 *  `borsht_type` - type of borsht to serve
 */
void serve(waitstaff_id waitstaff_id, table_id table_id,
           borsht_type borsht_type) {
  // TODO:
}

/*
 * Clean up table
 *
 * params:
 *  `table_id` - table to clean
 */
void clean_table(waitstaff_id w_id, table_id table_id) {
  // TODO:
}
