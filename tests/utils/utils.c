#include "utils.h"
extern state *GLOBAL_STATE;

customer *get_customer(customer_id id) {

  customer dummy;
  dummy.id = id;

  vector *customers = GLOBAL_STATE->customers;
  int customer_index = customers->find(customers, (void *)&dummy);

  customer *target;
  int _ = customers->get_mut_at(customers, customer_index, (void **)&target);
  return target;
}

table *get_table(table_id id) {
  table dummy;
  dummy.id = id;

  vector *tables = GLOBAL_STATE->tables;
  int table_index = tables->find(tables, (void *)&dummy);

  if (table_index == -1) {
    inconsistent_state();
    return NULL;
  }

  table *target;
  int _ = tables->get_mut_at(tables, table_index, (void **)&target);
  return target;
}

waitstaff *get_waitstaff(waitstaff_id id) {

  waitstaff dummy;
  dummy.id = id;

  vector *staff = GLOBAL_STATE->waitstaff_states;
  int staff_index = staff->find(staff, (void *)&dummy);

  if (staff_index == -1) {
    inconsistent_state();
    return NULL;
  }

  waitstaff *target;
  int _ = staff->get_mut_at(staff, staff_index, (void **)&target);

  return target;
}
