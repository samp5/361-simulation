#include "../sim.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void initializer();

void wait_staff_controller(waitstaff_id id);

void cook_controller();

void customer_controller(customer_id id);
