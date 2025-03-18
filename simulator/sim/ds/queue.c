#include "queue.h"
#include <stdlib.h>

static void add(queue *q, int element);
static int dequeue(queue *q, int *element);
static int size(queue *vec);
static void deallocate(queue **vec);
static void resize(queue *q);
static int get_at(queue *q, int, int *);

static void resize(queue *q) {
  int *new_arr = malloc(sizeof(int) * (q->capacity * 2));

  for (size_t i = 0; i < q->size; i++) {
    size_t index = (q->front + i) % q->capacity;
    new_arr[i] = q->arr[index];
  }

  q->capacity = q->capacity * 2;
  q->front = 0;

  free(q->arr);
  q->arr = new_arr;
}

void add(queue *q, int element) {
  if (q == NULL) {
    return;
  }
  if (q->size == q->capacity) {
    resize(q);
  }
  size_t index = (q->front + q->size) % q->capacity;
  q->arr[index] = element;
  q->size += 1;
}
int dequeue(queue *q, int *element) {
  if (q == NULL || q->size == 0) {
    return -1;
  }

  *element = q->arr[q->front];
  q->front = (q->front + 1) % q->capacity;
  q->size -= 1;

  return 0;
}

int get_at(queue *q, int index, int *element) {
  if (q == NULL || index >= q->size) {
    return -1;
  }
  size_t adj_index = (q->front + index) % q->capacity;
  *element = q->arr[adj_index];
  return 0;
}
int size(queue *q) {
  if (q == NULL) {
    return -1;
  }
  return q->size;
}
void deallocate(queue **q) {
  if (q == NULL) {
    return;
  }
  free((*q)->arr);
  free(*q);
  *q = NULL;
}

queue *new_queue() {
  queue *q = malloc(sizeof(queue));
  q->front = 0;
  q->size = 0;
  q->capacity = 10;
  q->arr = malloc(q->capacity * sizeof(int));

  // func ptrs
  q->len = size;
  q->dequeue = dequeue;
  q->queue = add;
  q->dealloc = deallocate;
  q->get_at = get_at;

  return q;
}
