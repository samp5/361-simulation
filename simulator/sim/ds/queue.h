#include <stdlib.h>
#ifndef DS_QUEUE
#define DS_QUEUE
typedef struct queue {
  int *_arr;
  size_t _front;
  size_t _capacity;
  size_t _size;
  int (*len)(struct queue *);
  int (*dequeue)(struct queue *, int *element);
  void (*queue)(struct queue *, int element);
  void (*dealloc)(struct queue **);
  int (*get_at)(struct queue *, int index, int *element);
  int (*peek)(struct queue *, int *element);
} queue;

queue *new_queue();

#endif // !DS_QUEUE
