#include "sim/controllers.h"
#include "sim.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *wait_staff() { pthread_exit(NULL); }

void *cook_staff() { pthread_exit(NULL); }

void *customer() { pthread_exit(NULL); }
