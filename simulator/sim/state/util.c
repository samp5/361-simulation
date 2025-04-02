#include "../log_macros.h"
#include "state.h"
#include "sync.h"
extern state *GLOBAL_STATE;

/*
 * NOTE: Lock.Customer must be held before this call and while the returned
 * customer is in scope
 *
 * If the requested customer does not exist, this foces the program to exit.
 * In the case of testing, this function should return NULL.
 */
customer *get_mut_customer(customer_id id) {

  if (!check(Customer)) {
    PRINT_ERR(0, "Cusomer mutex was not held on entering get_mut_customer%s",
              "\n");
  }

  customer dummy;
  dummy.id = id;

  vector *customers = GLOBAL_STATE->customers;
  int customer_index = customers->find(customers, (void *)&dummy);

  if (customer_index == -1) {
    LOG("Customer with ID = %d does not exists... exiting", id);
    inconsistent_state();
    return NULL;
  }

  customer *target;
  int _ = customers->get_mut_at(customers, customer_index, (void **)&target);
  return target;
}

/*
 * NOTE: The Lock.Tables must be held before this call and while the returned
 * pointer is in scope
 *
 * If the requested table does not exist, this foces the program to exit.
 * In the case of testing, this function should return NULL.
 */
table *get_mut_table(table_id id) {
  if (!check(Table)) {
    PRINT_ERR(0, "Cusomer mutex was not held on entering get_mut_customer%s",
              "\n");
  }
  table dummy;
  dummy.id = id;

  vector *tables = GLOBAL_STATE->tables;
  int table_index = tables->find(tables, (void *)&dummy);

  if (table_index == -1) {
    LOG("Table with ID = %d does not exists... exiting", id);
    inconsistent_state();
    return NULL;
  }

  table *target;
  int _ = tables->get_mut_at(tables, table_index, (void **)&target);
  return target;
}

/*
 * NOTE: Lock.Waitstaff must be held before this call and while the returned
 * waitstaff is in scope
 *
 * If the requested staff does not exist, this foces the program to exit.
 * In the case of testing, this function should return NULL.
 */
waitstaff *get_mut_waitstaff(waitstaff_id id) {

  if (!check(Waitstaff)) {
    PRINT_ERR(0, "Waitstaff mutex was not held on entering get_mut_waitstaff%s",
              "\n");
  }

  waitstaff dummy;
  dummy.id = id;

  vector *staff = GLOBAL_STATE->waitstaff_states;
  int staff_index = staff->find(staff, (void *)&dummy);

  if (staff_index == -1) {
    LOG("Staff member with ID = %d does not exists... exiting", id);
    inconsistent_state();
    return NULL;
  }

  waitstaff *target;
  int _ = staff->get_mut_at(staff, staff_index, (void **)&target);

  return target;
}

/*
 * NOTE: Lock.Queue and Lock.Customer must be held before this call
 *
 * This function adds a customer to the queue and updates their state.
 */
void add_customer_to_queue(customer_id id) {
  int locks = Queue | Customer;
  if (check(locks)) {
    PRINT_ERR(0,
              "Queue and Customer mutex was not held on entering "
              "add_customer_to_queue%s",
              "\n");
  }

  customer *custy;
  if ((custy = get_mut_customer(id)) == NULL) {
    PRINT_ERR(0, "Cutomer id did not exist in internal function? %s", "\n");
  }

  // Queue our customer
  custy->current_status = InQueue;
  GLOBAL_STATE->seating_line->queue(GLOBAL_STATE->seating_line, custy->id);
}

/*
 * NOTE: Lock.Queue and Lock.Customer must be held before this call
 *
 * This function removes a customer to the queue but does NOT update their
 * status as it cannot be detemined at this point what the next state should be
 */
void remove_customer_from_queue(customer_id id) {
  queue *q = GLOBAL_STATE->seating_line;

  int locks = Queue | Customer;
  if (!check(locks)) {
    PRINT_ERR(0,
              "Queue and Customer mutexes were not held on entering "
              "remove_customer_from_queue%s",
              "\n");
  }

  customer *custy;
  if ((custy = get_mut_customer(id)) == NULL) {
    PRINT_ERR(0, "Cutomer id did not exist in internal function? %s", "\n");
  }

  int first_in_line;
  // Dequeue our customer
  q->dequeue(GLOBAL_STATE->seating_line, &first_in_line);

  if (first_in_line != id) {
    PRINT_ERR(0, "Passed customer is not first in line in internal function%s",
              "\n");
  }
}
