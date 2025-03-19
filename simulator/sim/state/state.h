/*
 * This file contains functions for accessing the data structures that
 * track the state of the simulation.
 *
 */
#include "../ds/queue.h"
#include "../ds/vec.h"
#include <sys/types.h>
#ifndef STATE
#define STATE

typedef int16_t table_id;
typedef int customer_id;
typedef int16_t waitstaff_id;
typedef int8_t borsht_type;
typedef int customer_status;

typedef enum customer_statuses {
  NotArrived = 1, // before the customer has arrived
  InQueue = 2,    // waiting to be seated
  AtTable = 4,    // waiting to order
  Eating = 8,     // eating borsht
  Ordered = 16,   // after order has been taken
  Left = 32,
} customer_statuses;

typedef enum table_status {
  Clean,
  Occupied,
  Dirty,
} table_status;

/*
 * State of a table in the restaurant
 */
typedef struct table_state {
  table_id id;                 // id of the table
  table_status current_status; // status of the table
  int borsht_bowls[3];         // the number of prepared bowls indexed by type
} table;

typedef struct waitstaff_state {
  waitstaff_id id;
  borsht_type carrying[3]; // number of each type of borsht_type that this
                           // waiter is carrying
  vector *table_ids; // a collection of tables_ids that are to be managaed by
                     // this member of wait staff
} waitstaff;

typedef struct customer_state {
  customer_id id;                 // id of this customer
  borsht_type preference;         // borsht preference
  customer_status current_status; // bitwise & of customer statuses
  table_id table_id;
  int borsht_eaten;
  int borsht_desired;
} customer;

typedef struct order {
  borsht_type type; // type of borsht
  int quantity;     // number of this type
} order;

typedef struct kitchen_state {
  int prepared_bowls[3]; // the number of prepared bowls indexed by type
  vector *orders;        // a vector of orders (see type order)
  int num_cooks;
} kitchen;

typedef struct state {
  // customer state
  int num_customers;
  int num_customers_arrived;

  // customer states (cmp based on id)
  vector *customers;

  // customers who are waiting in line
  queue *seating_line;

  // waitstaff state
  vector *waitstaff_states;

  // all tables
  vector *tables;

  // kitchen state
  struct kitchen_state kitchen_state;

} state;

/* initialization */
state *init_state(int num_customers, int num_tables, int num_waiter,
                  int num_cooks);
vector *init_waitstaff_states(int num_waiter, int num_tables);
vector *init_customers(int customers);
kitchen init_kitchen_state(int num_cooks);
void init_customer_arrivals(state *state);
void delay_customer_arrival(customer_id id);
void dump_state(state *s);
void inconsistent_state();

#endif // !STATE
