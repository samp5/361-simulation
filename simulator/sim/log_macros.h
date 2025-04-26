#ifndef LOG_MACROS
#define LOG_MACROS

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static char buf[1024];
extern FILE *LOG_FILE;
#ifndef COLORS
#define COLORS

#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BOLD "\x1B[1m"

#endif // !COLORS

void log_init();
extern pthread_mutex_t log_tex;

#define BAIL_AND_RELEASE(msg, ...)                                             \
  do {                                                                         \
    LOG(msg, __VA_ARGS__);                                                     \
    release(locks);                                                            \
    inconsistent_state();                                                      \
    return;                                                                    \
  } while (0)

#define PRINT_ERR(recover, msg, ...)                                           \
  do {                                                                         \
    LOG(msg, __VA_ARGS__);                                                     \
    fprintf(stderr, RED BOLD msg RESET, __VA_ARGS__);                          \
    if (!recover) {                                                            \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

#define BAIL_RET_RELEASE(ret, msg, ...)                                        \
  do {                                                                         \
    LOG(msg, __VA_ARGS__);                                                     \
    release(locks);                                                            \
    inconsistent_state();                                                      \
    return ret;                                                                \
  } while (0)
#define LOG(msg, ...)                                                          \
  do {                                                                         \
    if (LOG_FILE == NULL) {                                                    \
      log_init();                                                              \
    }                                                                          \
    pthread_mutex_lock(&log_tex);                                              \
    sprintf(buf, msg, __VA_ARGS__);                                            \
    fprintf(LOG_FILE, "TID:%lu: %s\n", pthread_self(), buf);                   \
    fflush(LOG_FILE);                                                          \
    pthread_mutex_unlock(&log_tex);                                            \
  } while (0)

#endif // !LOG_MACROS
