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

int get_first_available_table(table **tbl, waitstaff **wstf) {
  vector *ws = GLOBAL_STATE->waitstaff_states;
  if (ws->len(ws) == 0)
    return -1;

  waitstaff *w = *wstf;
  table *t = *tbl;

  // find the first waitstaff with an open table
  for (int i = 0; i < ws->len(ws); i++) {
    ws->get_mut_at(ws, i, (void **)&w);

    // for each table for that waitstaff
    for (int j = 0; j < w->table_ids->len(w->table_ids); j++) {

      table_id t_id;
      w->table_ids->get_at(w->table_ids, j, (void *)&t_id);
      t = get_table(t_id);

      // break out if the table is unoccupied
      if (!(t->current_status & Occupied)) {
        break;
      }
    }

    // if we found an open table, break out!
    if (t != NULL && !(t->current_status & Occupied)) {
      break;
    }
  }

  // we didn't find an empty table, this is still valid
  if (t == NULL || t->current_status & Occupied) {
    return -1;
  }

  *tbl = t;
  *wstf = w;
  return 0;
}
