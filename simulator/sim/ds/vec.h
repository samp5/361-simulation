#include <stdlib.h>

typedef struct vector {
  void* arr;
  size_t capacity;
  size_t size;
  size_t elem_size;

  // func ptrs
  void (*push)(struct vector*, void*); 
  int (*pop)(struct vector*, void*); 
  int (*len)(struct vector*); 
  void (*dealloc) (struct vector**);
  int (*get_at)(struct vector*, size_t, void*);
  int (*find)(struct vector*, void*);
} vector;

vector* new_vector(size_t with_size);

