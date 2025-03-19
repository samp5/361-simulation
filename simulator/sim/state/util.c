#include "../log_macros.h"
#include "state.h"
#include "sync.h"
extern state *GLOBAL_STATE;

/*
 * NOTE: Lock.Customer must be held before this call and while the returned
 * customer is in scope
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
  }
  customer *target;
  int _ = customers->get_mut_at(customers, customer_index, (void **)&target);
  return target;
}

/*
 * NOTE: The Lock.Tables must be held before this call and while the returned
 * pointer is in scope
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
  }

  table *target;
  int _ = tables->get_mut_at(tables, table_index, (void **)&target);
  return target;
}

/*
 * NOTE: Lock.Waitstaff must be held before this call and while the returned
 * waitstaff is in scope
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
  }

  waitstaff *target;
  int _ = staff->get_mut_at(staff, staff_index, (void **)&target);

  return target;
}
