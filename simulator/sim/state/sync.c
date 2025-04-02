
#include "sync.h"
#include <pthread.h>

static pthread_mutex_t customer_tex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t table_tex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t waitstaff_tex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t cook_tex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t queue_tex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t global_tex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Always take locks in this order
 */
void take(int locks) {
  if (locks == Global) {
    pthread_mutex_lock(&global_tex);
    return;
  }
  if (Customer & locks) {
    pthread_mutex_lock(&customer_tex);
  }
  if (Table & locks) {
    pthread_mutex_lock(&table_tex);
  }
  if (Waitstaff & locks) {
    pthread_mutex_lock(&waitstaff_tex);
  }
  if (Cook & locks) {
    pthread_mutex_lock(&cook_tex);
  }
  if (Queue & locks) {
    pthread_mutex_lock(&cook_tex);
  }
}

#define MUTEX_CHECK(tex, res)                                                  \
  do {                                                                         \
    if (pthread_mutex_trylock(&tex) == 0) {                                    \
      pthread_mutex_unlock(&tex);                                              \
      res = 0;                                                                 \
    } else {                                                                   \
      res = 1;                                                                 \
    }                                                                          \
  } while (0)

int check(int locks) {
  int result = 0;
  MUTEX_CHECK(global_tex, result);
  if (result) {
    return result;
  }
  if (Customer & locks) {
    MUTEX_CHECK(customer_tex, result);
  }
  if (Table & locks) {
    MUTEX_CHECK(table_tex, result);
  }
  if (Waitstaff & locks) {
    MUTEX_CHECK(waitstaff_tex, result);
  }
  if (Cook & locks) {
    MUTEX_CHECK(cook_tex, result);
  }
  if (Queue & locks) {
    MUTEX_CHECK(queue_tex, result);
  }
  return result;
}

/*
 * Always release locks in the opposite order
 */
void release(int locks) {
  if (locks == Global) {
    pthread_mutex_unlock(&global_tex);
    return;
  }
  if (Queue & locks) {
    pthread_mutex_unlock(&cook_tex);
  }
  if (Cook & locks) {
    pthread_mutex_unlock(&cook_tex);
  }
  if (Waitstaff & locks) {
    pthread_mutex_unlock(&waitstaff_tex);
  }
  if (Table & locks) {
    pthread_mutex_unlock(&table_tex);
  }
  if (Customer & locks) {
    pthread_mutex_unlock(&customer_tex);
  }
}
