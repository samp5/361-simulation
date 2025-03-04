#include <stdlib.h>
#include <sys/types.h>

typedef u_int16_t table_id;
typedef u_int16_t customer_id;
typedef u_int8_t borsht_type;

/////////////////////////////////////////////////////////////////////
//
// Customer API
//

/*
 * Leave the restaurant
 */
void leave();

/*
 * Eat borsht
 */
void eat();

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
void get_tables(table_id **tables, size_t *length);

/*
 * Seat a customer at a table
 *
 * params:
 *  `customer_id` - id of customer
 *  `table_id` - id of table
 *
 * return:
 *  integer representing the borsht type preferred by
 *  customer with `customer_id`
 */
void seat(customer_id customer_id, table_id table_id);

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
 *  `table_id` - id of table
 *
 * return by assignment:
 *  `quantitiy` - number of bowls requested by customer
 *
 */
void take_order(table_id table_id, int *quantitiy);

/*
 * Pick up borsht from the kitchen
 *
 * params:
 *  `borsht_type` - type of borsht to pick up
 *  `quantitiy` - number of bowls
 *
 */
void pick_up_borsht(borsht_type borsht_type, int quantitiy);

/*
 * Pick up borsht from the kitchen
 *
 * params:
 *  `table_id` - table to serve to
 *  `borsht_type` - type of borsht to serve
 */
void serve(table_id table_id, borsht_type borsht_type);

/*
 * Clean up table
 *
 * params:
 *  `table_id` - table to serve to
 */
void clean_table(table_id table_id);

/////////////////////////////////////////////////////////////////////
//
// Cook API
//

/*
 * Prepare some borsht
 *
 * params:
 *  `borsht_type` - type of borsht
 *  `quantitiy` - number of bowls to prepare
 */
void prepare(int borsht_type, int quantitiy);
