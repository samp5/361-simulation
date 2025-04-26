#include "../../tests/macros.h"
#include "log_macros.h"
#include "state/state.h"
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
FILE *LOG_FILE = NULL;
extern state *GLOBAL_STATE;
pthread_mutex_t log_tex = PTHREAD_MUTEX_INITIALIZER;

void log_init() {
  static int run = 0;
  char filename[32];
  TEST_CONTEXT_F("  log %d...", run);
  snprintf(filename, 32, "logs/log%d.txt", run++);
  if ((LOG_FILE = fopen(filename, "w")) == NULL) {
    perror(filename);
    exit(1);
  };
}
