#ifndef TEST_UTIL
#define TEST_UTIL
#include "../../../simulator/sim/state/state.h"

customer *get_customer(customer_id);
table *get_table(table_id id);
waitstaff *get_waitstaff(waitstaff_id id);

// return 0 when we found one, -1 otherwise
int get_first_available_table(table **t, waitstaff **w);
#endif // !TEST_UTIL
