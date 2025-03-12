/*
 * This file contains functions for accessing the data structures that
 * track the state of the simulation. 
 * 
*/
#ifndef STATE
#define STATE
#include "../ds/vec.h"
#include "../ds/queue.h"
#include <sys/types.h>

typedef u_int16_t table_id;
typedef u_int16_t customer_id;
typedef u_int16_t waitstaff_id;
typedef u_int8_t borsht_type;


typedef enum customer_status {
  NotArrived,
  Eating,
  Waiting,
  Left,
} customer_status;

typedef enum table_status {
  Clean,
  Occupied,
  Dirty,
} table_status;

/*
 * State of a table in the restaurant
 */
typedef struct table_state{
  table_id id;
  table_status current_status;
} table;

typedef struct waitstaff_state {
  waitstaff_id id;
  borsht_type carrying[3];
  vector* tables; 
} waitstaff;

typedef struct customer_state {
  customer_id id;
  borsht_type preference;
  customer_status current_status;
} customer;

typedef struct order {
  borsht_type type;
  int quantity;
} order;

typedef struct kitchen_state {
  int prepared_bowls[3]; // the number of prepared bowls indexed by type
  vector* orders;
} kitchen;



typedef struct state {
  // customer state
  int num_customers;
  int num_customers_arrived;

  // customer states
  vector* customers;

  // customers who are waiting in line (based on customer_id)
  queue* seating_line;

  // waitstaff state
  vector* waitstaff_states;

  // all table ids
  vector* table_ids;

  // kitchen state
  struct kitchen_state kitchen_state;
  
}state;

state* init_state(int num_customers, int num_tables, int num_waiter, int num_cooks);
vector* init_waitstaff_states(int num_waiter, int num_tables);
vector* init_tables(int num_tables, table_id start);
vector* init_customers(int customers);
kitchen init_kitchen_state();
#endif // !STATE
