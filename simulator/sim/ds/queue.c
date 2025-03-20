#include "queue.h"
#include <stdlib.h>

static void add(queue *q, int element);
static int dequeue(queue *q, int *element);
static int size(queue *vec);
static void deallocate(queue **vec);
static void resize(queue *q);
static int get_at(queue *q, int, int *);

static void resize(queue *q) {
  int *new_arr = malloc(sizeof(int) * (q->_capacity * 2));

  for (size_t i = 0; i < q->_size; i++) {
    size_t index = (q->_front + i) % q->_capacity;
    new_arr[i] = q->_arr[index];
  }

  q->_capacity = q->_capacity * 2;
  q->_front = 0;

  free(q->_arr);
  q->_arr = new_arr;
}

void add(queue *q, int element) {
  if (q == NULL) {
    return;
  }
  if (q->_size == q->_capacity) {
    resize(q);
  }
  size_t index = (q->_front + q->_size) % q->_capacity;
  q->_arr[index] = element;
  q->_size += 1;
}
int dequeue(queue *q, int *element) {
  if (q == NULL || q->_size == 0) {
    return -1;
  }

  *element = q->_arr[q->_front];
  q->_front = (q->_front + 1) % q->_capacity;
  q->_size -= 1;

  return 0;
}

int get_at(queue *q, int index, int *element) {
  if (q == NULL || index >= (int)q->_size) {
    return -1;
  }
  size_t adj_index = (q->_front + index) % q->_capacity;
  *element = q->_arr[adj_index];
  return 0;
}

int peek(queue *q, int *element) {
  if (q == NULL || q->_size == 0) {
    return -1;
  }
  *element = q->_arr[q->_front];
  return 0;
}
int size(queue *q) {
  if (q == NULL) {
    return -1;
  }
  return q->_size;
}
void deallocate(queue **q) {
  if (q == NULL) {
    return;
  }
  free((*q)->_arr);
  free(*q);
  *q = NULL;
}

queue *new_queue() {
  queue *q = malloc(sizeof(queue));
  q->_front = 0;
  q->_size = 0;
  q->_capacity = 10;
  q->_arr = malloc(q->_capacity * sizeof(int));

  // func ptrs
  q->len = size;
  q->dequeue = dequeue;
  q->queue = add;
  q->dealloc = deallocate;
  q->get_at = get_at;
  q->peek = peek;

  return q;
}
