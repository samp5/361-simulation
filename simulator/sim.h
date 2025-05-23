#include <stdlib.h>
#include <sys/types.h>
extern const int NUM_BORSHT_TYPE;

typedef int16_t table_id;
typedef int customer_id;
typedef int8_t borsht_type;
typedef int waitstaff_id;

/////////////////////////////////////////////////////////////////////
//
// Global API
void get_waiting_line(customer_id **arr, size_t *len);

/////////////////////////////////////////////////////////////////////
//
// Customer API
//

/*
 * Check to see if a customer is in the restaurant
 */
int has_arrived(customer_id);

/*
 * Leave the restaurant
 */
void leave(customer_id);

/*
 * Eat borsht
 */
void eat(customer_id);

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
void get_tables(waitstaff_id id, table_id **tables, size_t *length);

/*
 * Seat a customer at a table.
 * Prior to seating a customer they should have been at the front of the queue.
 *
 * params:
 *  `customer_id` - id of customer
 *  `table_id` - id of table
 *
 */
void seat(waitstaff_id waitstaff_id, customer_id customer_id,
          table_id table_id);

/*
 * Get the borsht preference of a customer
 *
 * params:
 *  `customer_id` - id of customer 🧠
 *
 * return:
 *  `borsht_type` representing the borsht type preferred by
 *  customer with `c_id`
 */
borsht_type intuit_preference(customer_id c_id);

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

// TODO:
/*
 * Pick up borsht from the kitchen
 *
 * params:
 *  `borsht_type` - type of borsht to pick up
 *  `quantitiy` - number of bowls
 *
 */
void pick_up_borsht(waitstaff_id id, borsht_type borsht_type, int quantitiy);

// TODO:
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

/////////////////////////////////////////////////////////////////////
//
// Cook API
//

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
void prepare(int borsht_type, int quantitiy);
