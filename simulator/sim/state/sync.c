
#include "sync.h"
#include <pthread.h>

static pthread_mutex_t customer_tex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t table_tex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t waitstaff_tex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t cook_tex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Always take locks in this order
 */
void take(int locks) {
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
}

/*
 * Always release locks in the opposite order
 */
void release(int locks) {
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
