#include "controllers.h"
#include "state/state.h"
#include <pthread.h>
#include <stdio.h>

void inconsistent_state(){
  exit(1);
}

int main(int argc, char **argv) { printf("this restaurant sucks\n"); }
