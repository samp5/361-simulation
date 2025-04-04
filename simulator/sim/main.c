#include "controllers.h"
#include "state/state.h"
#include <pthread.h>
#include <stdio.h>

void inconsistent_state() { exit(1); }
state *GLOBAL_STATE;

int main(int argc, char **argv) {
  GLOBAL_STATE = init_state(0, 0, 0, 0);
  printf("this restaurant sucks\n");
}
