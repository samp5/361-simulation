#include "state.h"
#include <stdlib.h>
const int BORSHT_TYPES = 3;

vector* init_waitstaff_states(int num_waiter, int num_tables);
vector* init_tables(int num_tables, table_id start);
vector* init_customers(int customers);
kitchen init_kitchen_state();

state* init_state(int num_customers, int num_tables, int num_waiter, int num_cooks){
  // allocate state
  state* sim_state = malloc(sizeof(state));

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
  sim_state->table_ids = new_vector(sizeof(table_id));

  // give all tables an id
  for (int i = 0; i < num_tables; i++) {
    table_id* table_i = malloc(sizeof(table_id));
    sim_state->table_ids->push(sim_state->table_ids, (void*)table_i);
  }

  // kitchen state
  sim_state->kitchen_state = init_kitchen_state();

  return sim_state;
}

/*
 * initialize a vector of waitstaff members based on a number of total waiters and 
 * a number of tables
 *
 * param: num_waiter number of waiters to create
 * param: num_tables the total number of tables in the restaurant
 * 
 * returns vector of waitstaff elements
 */
vector* init_waitstaff_states(int num_waiter, int num_tables){
  int tables_per_waitstaff;
  int excess_tables;

  if (num_waiter != 0){
    tables_per_waitstaff = num_tables / num_waiter;
    excess_tables = num_tables % num_waiter;
  }

  vector* waitstaff_states = new_vector(sizeof(waitstaff));

  table_id start = 0;

  for (int i = 0; i < num_waiter; i++){
    int tables_for_i = tables_per_waitstaff;

    if (excess_tables){
      tables_for_i += 1;
      excess_tables -= 1;
    }

    // allocate new waiter
    waitstaff* waiter_i = malloc(sizeof(waitstaff));

    // set waiters params
    waiter_i->id = i; // id
    for (int b = 0; b < BORSHT_TYPES; b++){ // carrying nothing
      waiter_i->carrying[b] = 0;
    }
    waiter_i->tables = init_tables(tables_for_i, start); // tables 

    waitstaff_states->push(waitstaff_states, (void*)waiter_i);  // add this waiter
    start = start + tables_for_i; // increment our table_id
  }
  return waitstaff_states;
}

/*
 * initialize a vector of tables 
 *
 * param: num_tables number of tables to create
 * param: start the starting table_id to use when creating tables
 */
vector* init_tables(int num_tables, table_id start) {
  vector* tables = new_vector(sizeof(table));
  table_id id_i = start;

  for (int i = 0; i < num_tables; i++){
    table* table_i = malloc(sizeof(table));
    table_i->current_status = Clean;
    table_i->id = id_i;
    id_i = id_i + 1;

    tables->push(tables, (void*)table_i);
  }

  return tables;
}

vector* init_customers(int num_customers){
  vector* customers = new_vector(sizeof(customer));

  for (int i = 0; i < num_customers; i++){
    customer* customer_i = malloc(sizeof(customer));
    customer_i->id = i;
    customer_i->preference = rand() % BORSHT_TYPES;
    customer_i->current_status = NotArrived;
    customers->push(customers, customer_i);
  }

  return customers;
}

kitchen init_kitchen_state() {
  // kitchen_state
  kitchen kitchen_state;

  // set all prepared_bowls  to zero
  for (int i =0; i < BORSHT_TYPES; i++){
    kitchen_state.prepared_bowls[i] = 0;
  }

  // no current orders
  kitchen_state.orders = new_vector(sizeof(order));

  return kitchen_state;
}
