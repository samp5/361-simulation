#include "../simulator/sim/ds/vec.h"
#include "macros.h"
#include "vec_test.h"
#include <stdio.h>

char str[1024];
static int test_empty();
static int test_single_int();
static int test_single_struct();
static int test_many_int();

void vector_test_all(){
  TEST(test_empty);
  TEST(test_single_int);
  TEST(test_many_int);
  TEST(test_single_struct);
}

int test_empty(){
  vector* v = new_vector(sizeof(int));
  ASSERT(v->size == 0, "Expected new vector to have empty size");
  ASSERT(v->get_at(v, 0, NULL) == -1, "Failed to prevent out of range index");
  ASSERT(v->find(v, NULL) == -1, "Found non-existent element");
  return 0;
}

int test_single_int(){
  vector* v = new_vector(sizeof(int));
  int x = 1;
  v->push(v, (void*)&x);
  ASSERT(v->len(v) == 1, "Expected new vector to have empty size");

  int val = 0;
  int res = v->get_at(v, 0, (void*)&val);
  ASSERT(res == 0, "Failed to find index 1");
  ASSERT(val == 1, "Incorrect element retrieved");

  int index; 
  res = v->find(v, (void*)&val);
  ASSERT(res == 0, "Failed to find index 1");

  res = v->pop(v, (void*)&val);
  ASSERT(res == 0, "Failed pop");
  ASSERT(val == 1, "Incorrect element retrieved");

  ASSERT(v->len(v) == 0, "Expected zero size after pop");

  return 0;
}

int test_many_int(){
  vector* v = new_vector(sizeof(int));
  for (int i = 0; i < 200; i++){
    v->push(v, (void*)&i);
  }
  ASSERT(v->len(v) == 200, "Expected new vector to have empty size");

  for (int i = 0; i < 200; i++){
    int val = 0;
    int res = v->get_at(v, i, (void*)&val);
    sprintf(str,"Failed to get index %d",i);
    ASSERT(res == 0, str);
    sprintf(str,"Incorrect element retrieved: got %d expected %d",val, i);
    ASSERT(val == i, str);
  }

  for (int i = 0; i < 200; i++){
    int res = v->find(v, (void*)&i);
    sprintf(str," Wrong index for value %d: got %d expected %d", i, res, i);
    ASSERT(res == i, str);
  }

  for (int i = 199; i >= 0; i--){
    int end;
    int res= v->pop(v, (void*)&end);
    ASSERT(res == 0, "Failed pop");
    ASSERT(end == i, "Incorrect element retrieved");
  }

  ASSERT(v->len(v) == 0, "Expected zero size after pop");

  return 0;
}

struct point {
  int x;
  int y;
};

int test_single_struct(){
  vector* v = new_vector(sizeof(struct point));
  struct point pt;
  pt.x = 1;
  pt.y = 2;

  v->push(v, (void*)&pt);
  ASSERT(v->len(v) == 1, "Expected new vector to have empty size");

  struct point val;
  int res = v->get_at(v, 0, (void*)&val);
  ASSERT(res == 0, "Failed to find index 1");
  ASSERT(val.x == 1 && val.y == 2, "Incorrect element retrieved");

  int index; 
  res = v->find(v, (void*)&pt);
  ASSERT(res == 0, "Failed to find index 1");

  res = v->pop(v, (void*)&val);
  ASSERT(res == 0, "Failed pop");
  ASSERT(val.x == 1 && val.y == 2, "Incorrect element retrieved");

  ASSERT(v->len(v) == 0, "Expected zero size after pop");

  return 0;
}
