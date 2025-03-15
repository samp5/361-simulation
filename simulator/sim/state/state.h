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
  NotArrived, // before the customer has arrived
  InQueue, // waiting to be seated
  WaitingAtTable, // waiting at the table (either ready to order or waiting for food)
  Eating, // eating borsht
  Ordered,        // after order has been taken
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
  table_id id; // id of the table
  table_status current_status; // status of the table
} table;

typedef struct waitstaff_state {
  waitstaff_id id;
  borsht_type carrying[3]; // number of each type of borsht_type that this 
                           // waiter is carrying
  vector* tables;  // a collection of tables that are to be managaed by this 
                   // member of wait staff
} waitstaff;

typedef struct customer_state {
  customer_id id; // id of this customer
  borsht_type preference; // borsht preference 
  customer_status current_status; // current customer status
} customer;

typedef struct order {
  borsht_type type; // type of borsht
  int quantity; // number of this type
} order; 

typedef struct kitchen_state {
  int prepared_bowls[3]; // the number of prepared bowls indexed by type
  vector* orders; // a vector of orders (see type order)
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
vector* init_customers(int customers);
kitchen init_kitchen_state();
#endif // !STATE
