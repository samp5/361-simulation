#include "vec.h"
#include <stdlib.h>
#include <string.h>

void push(vector* vec, void* element);
int pop(vector* vec, void* element);
int size(vector* vec);
void deallocate(vector** vec);
int get_at(vector* vec, size_t index, void* element);
int find(vector* vec, void* element);

static void resize(vector* vec){
  void* new_arr = realloc(vec->arr, vec->elem_size*(vec->capacity * 2));
  if (new_arr != vec->arr){
    vec->arr = new_arr;
  }
  vec->capacity = vec->capacity * 2;
}

void push(vector* vec, void* element) {
  if (vec->size  == vec->capacity){
    resize(vec);
  }
  memcpy(vec->arr + (vec->size * vec->elem_size), element, vec->elem_size);
  vec->size += 1;
}

/*
 * 0 returned on sucess
 * -1 is returned on error
 *
 *  element will hold back element 
*/
int pop(vector* vec, void* element){
  if (vec->size == 0 || vec == NULL){
    return -1;
  }
  memcpy(element, vec->arr + ((vec->size - 1) * vec->elem_size), vec->elem_size);
  vec->size -= 1;
  return 0;
}

int size(vector* vec) {
  if (vec == NULL){
    return -1;
  }
  return vec->size;
}
void deallocate(vector** vec){
  if (vec == NULL){
    return;
  }
  free((*vec)->arr);
  free(*vec);
  *vec = NULL;
}

/*
 * 0 returned on sucess
 * -1 is returned on error
 *
 *  element will hold back element 
*/
int get_at(vector* vec, size_t index, void* element){
  if (vec == NULL || index >= vec->size){
    return -1;
  }
  memcpy(element, vec->arr + (index * vec->elem_size), vec->elem_size);
  return 0;
}

int find(vector* vec, void* element){
  if (vec == NULL){
    return -1;
  }
  if (vec->compare != NULL){
    for (size_t i = 0; i < vec->size; i++){
      if (vec->compare(vec->arr + (i * vec->elem_size), element) == 0){
        return i;
      }
    }
    return -1;
  } else {
    for (size_t i = 0; i < vec->size; i++){
      if (!memcmp(vec->arr + (i * vec->elem_size), element , vec->elem_size)){
        return i;
      }
    }
    return -1;
  }
}

static void set_comparator(struct vector* v, int (*compare)(void*, void*)){
  v->compare = compare;
}

vector* new_vector(size_t with_size) {
  vector* vec = malloc(sizeof(vector));
  vec->capacity = 10;
  vec->arr = malloc(vec->capacity * (with_size));
  vec->size = 0;
  vec->elem_size = with_size;

  vec->push = push;
  vec->pop = pop ;
  vec->len = size;
  vec->dealloc = deallocate;
  vec->get_at = get_at ;
  vec->find = find;
  vec->set_comparator = set_comparator;

  vec->compare = NULL;

  return vec;
}
