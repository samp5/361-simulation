#ifndef LOG_MACROS
#define LOG_MACROS

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static char buf[1024];
extern FILE *LOG_FILE;

void log_init();

#define BAIL_AND_RELEASE(msg, ...)                                             \
  do {                                                                         \
    LOG(msg, __VA_ARGS__);                                                     \
    release(locks);                                                            \
    inconsistent_state();                                                      \
    return;                                                                    \
  } while (0)
#define BAIL_RET_RELEASE_OHMY(ret, msg, ...)                                   \
  do {                                                                         \
    LOG(msg, __VA_ARGS__);                                                     \
    release(locks);                                                            \
    return ret;                                                                \
  } while (0)
#define LOG(msg, ...)                                                          \
  do {                                                                         \
    if (LOG_FILE == NULL) {                                                    \
      log_init();                                                              \
    }                                                                          \
    sprintf(buf, msg, __VA_ARGS__);                                            \
    fprintf(LOG_FILE, "TID:%lu: %s\n", pthread_self(), buf);                   \
  } while (0)

#endif // !LOG_MACROS
