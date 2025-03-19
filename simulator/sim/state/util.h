#ifndef STATE_UTIL
#define STATE_UTIL

#include "state.h"

/*
 * NOTE: Lock.Customer must be held before this call and while the returned
 * customer is in scope
 *
 * If the requested customer does not exist, this foces the program to exit.
 * In the case of testing, this function should return NULL.
 */
customer *get_mut_customer(customer_id);

/*
 * NOTE: The Lock.Tables must be held before this call and while the returned
 * pointer is in scope
 *
 * If the requested table does not exist, this foces the program to exit.
 * In the case of testing, this function should return NULL.
 */
table *get_mut_table(table_id id);
/*
 * NOTE: Lock.Waitstaff must be held before this call and while the returned
 * waitstaff is in scope
 *
 * If the requested staff does not exist, this foces the program to exit.
 * In the case of testing, this function should return NULL.
 */
waitstaff *get_mut_waitstaff(waitstaff_id id);

/*
 * NOTE: Lock.Queue and Lock.Customer must be held before this call
 *
 * This function adds a customer to the queue and updates their state.
 */
void add_customer_to_queue(customer_id);

/*
 * NOTE: Lock.Queue and Lock.Customer must be held before this call
 *
 * This function removes a customer to the queue but does NOT update their
 * status as it cannot be detemined at this point what the next state should be
 */
void remove_customer_from_queue(customer_id);

#endif // !STATE_UTIL
