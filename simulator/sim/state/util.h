#ifndef STATE_UTIL
#define STATE_UTIL

#include "state.h"
customer *get_mut_customer(customer_id);
table *get_mut_table(table_id id);
waitstaff *get_mut_waitstaff(waitstaff_id id);

#endif // !STATE_UTIL
