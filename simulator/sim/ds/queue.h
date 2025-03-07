#include <stdlib.h>

typedef struct queue {
  int* arr;
  size_t front;
  size_t capacity;
  size_t size;
  int (*len)(struct queue*); 
  int (*dequeue)(struct queue*, int* element); 
  void (*queue)(struct queue*, int element); 
  void (*dealloc) (struct queue**);
} queue;

queue* new_queue();

