#include "log_macros.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
FILE* LOG_FILE  = NULL;

void log_init(){
  if ((LOG_FILE = fopen("log.txt", "w")) == NULL){
    perror("failed to open log.txt");
    exit(1);
  };
}
