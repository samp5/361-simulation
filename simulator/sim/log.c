#include "log_macros.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
FILE *LOG_FILE = NULL;

void log_init() {
  static int run = 0;
  char filename[32];
  snprintf(filename, 32, "log%d.txt", run++);
  if ((LOG_FILE = fopen(filename, "w")) == NULL) {
    perror(filename);
    exit(1);
  };
}
