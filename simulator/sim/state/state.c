#include "state.h"
#include <stdio.h>
#include <stdlib.h>
const int BORSHT_TYPES = 3;

vector *init_waitstaff_states(int num_waiter, int num_tables);
vector *init_tables(int num_tables);
vector *init_customers(int customers);
kitchen init_kitchen_state(int);

state *init_state(int num_customers, int num_tables, int num_waiter,
                  int num_cooks) {
  // allocate state
  state *sim_state = malloc(sizeof(state));

  // fill in basic counter variables
  sim_state->num_customers = num_customers;
  sim_state->num_customers_arrived = 0;

  // customer collection
  sim_state->customers = init_customers(num_customers);

  // seating queue
  sim_state->seating_line = new_queue();

  // waitstaff collection
  sim_state->waitstaff_states = init_waitstaff_states(num_waiter, num_tables);

  // table collection
  sim_state->tables = init_tables(num_tables);

  // kitchen state
  sim_state->kitchen_state = init_kitchen_state(num_cooks);

  return sim_state;
}

/*
 * initialize a vector of waitstaff members based on a number of total waiters
 * and a number of tables
 *
 * param: num_waiter number of waiters to create
 * param: num_tables the total number of tables in the restaurant
 *
 * returns vector of waitstaff elements
 */
vector *init_waitstaff_states(int num_waiter, int num_tables) {
  int tables_per_waitstaff;
  int excess_tables;

  if (num_waiter != 0) {
    tables_per_waitstaff = num_tables / num_waiter;
    excess_tables = num_tables % num_waiter;
  }

  vector *waitstaff_states = new_vector(sizeof(waitstaff));

  table_id start = 0;

  for (int i = 0; i < num_waiter; i++) {
    int tables_for_i = tables_per_waitstaff;

    if (excess_tables) {
      tables_for_i += 1;
      excess_tables -= 1;
    }

    // allocate new waiter
    waitstaff *waiter_i = malloc(sizeof(waitstaff));

    // set waiters params
    waiter_i->id = i;                        // id
    for (int b = 0; b < BORSHT_TYPES; b++) { // carrying nothing
      waiter_i->carrying[b] = 0;
    }

    // allocate waiter's table vector
    waiter_i->table_ids = new_vector(sizeof(table_id));

    // fill with appropriate table ids
    for (table_id i = start; i < start + tables_for_i; i++) {
      table_id *id_i = malloc(sizeof(table_id));
      *id_i = i;
      waiter_i->table_ids->push(waiter_i->table_ids, (void *)id_i);
    }

    waitstaff_states->push(waitstaff_states,
                           (void *)waiter_i); // add this waiter
    start = start + tables_for_i;             // increment our table_id
  }
  return waitstaff_states;
}

/*
 * Compaire function for table vector
 * based on table_id
 */
int table_cmp(void *a, void *b) {
  table *table_a = (table *)a;
  table *table_b = (table *)b;
  if (table_a->id == table_b->id) {
    return 0;
  } else {
    return -1;
  }
}

/*
 * initialize a vector of tables
 *
 * param: num_tables number of tables to create
 * param: start the starting table_id to use when creating tables
 */
vector *init_tables(int num_tables) {
  vector *tables = new_vector(sizeof(table));
  table_id id_i = 0;

  for (int i = 0; i < num_tables; i++) {
    table *table_i = malloc(sizeof(table));
    table_i->current_status = Clean;
    table_i->id = id_i;
    for (int i = 0; i < BORSHT_TYPES; i++) {
      table_i->borsht_bowls[i] = 0;
    }
    id_i = id_i + 1;

    tables->push(tables, (void *)table_i);
  }
  tables->set_comparator(tables, table_cmp);

  return tables;
}

int customer_cmp(void *a, void *b) {
  customer *customer_a = (customer *)a;
  customer *customer_b = (customer *)b;
  if (customer_a->id == customer_b->id) {
    return 0;
  } else {
    return -1;
  }
}

vector *init_customers(int num_customers) {

  vector *customers = new_vector(sizeof(customer));

  for (int i = 0; i < num_customers; i++) {
    customer *customer_i = malloc(sizeof(customer));
    customer_i->id = i;
    customer_i->preference = rand() % BORSHT_TYPES;
    customer_i->current_status = NotArrived;
    customer_i->borsht_eaten = 0;
    customer_i->table_id = -1;
    customers->push(customers, customer_i);
  }

  customers->set_comparator(customers, customer_cmp);

  return customers;
}

kitchen init_kitchen_state(int num_cooks) {
  // kitchen_state
  kitchen kitchen_state;

  // set all prepared_bowls  to zero
  for (int i = 0; i < BORSHT_TYPES; i++) {
    kitchen_state.prepared_bowls[i] = 0;
  }

  // no current orders
  kitchen_state.orders = new_vector(sizeof(order));
  kitchen_state.num_cooks = num_cooks;

  return kitchen_state;
}

void init_customer_arrivals(state *state) {
  vector *customers = state->customers;
  for (int i = 0; i < customers->len(customers); i++) {
    customer c_i;
    customers->get_at(customers, i, (void *)&c_i);
    delay_customer_arrival(c_i.id);
  }
}

void dump_state(state *s) {
  char fn[1024];
  sprintf(fn, "state_%d.txt", s->num_customers);
  FILE *f = fopen(fn, "w");

  fprintf(f, "Global State:\n");
  fprintf(f, "  Basic Statistics:\n");
  fprintf(f, "    Number Customers: %d\n", s->num_customers);
  fprintf(f, "    Number Arrived:   %d\n", s->num_customers_arrived);
  fprintf(f, "  Customers:\n");
  for (int i = 0; i < s->customers->len(s->customers); i++) {
    customer c_i;
    s->customers->get_at(s->customers, i, (void *)&c_i);
    fprintf(f, "    ID: %-3d BP: %-1d Stat: %-3d TID: %-3d Eaten: %-3d\n",
            c_i.id, c_i.preference, c_i.current_status, c_i.table_id,
            c_i.borsht_eaten);
  }

  // TODO: Give queue an iter
  fprintf(f, "  In Queue:");
  for (int i = 0; i < s->seating_line->len(s->seating_line); i++) {
    if (i % 30 == 0) {
      fprintf(f, "\n    ");
    }
    int id;
    s->seating_line->get_at(s->seating_line, i, &id);
    fprintf(f, "%-3d", id);
  }
  fprintf(f, "\n");

  fprintf(f, "  Waitstaff:\n");
  for (int i = 0; i < s->waitstaff_states->len(s->waitstaff_states); i++) {
    waitstaff w_i;
    s->waitstaff_states->get_at(s->waitstaff_states, i, (void *)&w_i);
    fprintf(f, "    ID: %-3d Carrying: [%-1d,%-1d,%-1d]\n", w_i.id,
            w_i.carrying[0], w_i.carrying[1], w_i.carrying[2]);
    fprintf(f, "    Tables:");
    for (int j = 0; j < w_i.table_ids->len(w_i.table_ids); j++) {
      if (j % 30 == 0) {
        fprintf(f, "\n    ");
      }
      table_id id;
      w_i.table_ids->get_at(w_i.table_ids, j, (void *)&id);
      fprintf(f, "%-3d", id);
    }
    fprintf(f, "\n");
  }
  fprintf(f, "\n");

  fprintf(f, "  Tables:\n");
  for (int i = 0; i < s->tables->len(s->tables); i++) {
    table t_i;
    s->tables->get_at(s->tables, i, (void *)&t_i);
    fprintf(f, "    ID: %-3d Status: %-1d Bowls: [%-1d,%-1d,%-1d]\n", t_i.id,
            t_i.current_status, t_i.borsht_bowls[0], t_i.borsht_bowls[1],
            t_i.borsht_bowls[2]);
  }

  fprintf(f, "  Kitchen:\n");
  fprintf(
      f, "    Ready: [%-1d,%-1d,%-1d]\n", s->kitchen_state.prepared_bowls[0],
      s->kitchen_state.prepared_bowls[1], s->kitchen_state.prepared_bowls[2]);

  fprintf(f, "    Orders:");
  for (int i = 0; i < s->kitchen_state.orders->len(s->kitchen_state.orders);
       i++) {
    if (i % 30 == 0) {
      fprintf(f, "\n    ");
    }
    order o_i;
    s->kitchen_state.orders->get_at(s->kitchen_state.orders, i, (void *)&o_i);
    fprintf(f, "    Type: %-2d Quantity: %-3d", o_i.type, o_i.quantity);
  }
  fprintf(f, "\n");
  fclose(f);
}
