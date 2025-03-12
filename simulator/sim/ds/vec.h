#include <stdlib.h>

// NOTE:
// this vector implemenation should only store
// HEAP allocated elements. 
//
// Because the vector only stores *pointers* to it's elements
// storing stack allocated variables in this vector is almost
// guarenteed to produce undefined behavior.
//
// NOTE: If elements contain internal pointers, they are not freed
// upon vector deallocation. 
//
// Only the underlying array and struct are freed by this impl.
// 
typedef struct vector {
  void* arr;
  size_t capacity;
  size_t size;
  size_t elem_size;

  /*
   * Push an element onto the back of the vector
   *
   * param: vec this
   *
   * param: elem element to push
   *
  */
  void (*push)(struct vector* vec, void* elem); 

  /*
   * pop the last element off the vector
   *
   * param: vec this
   *
   * param return by assignment: copies the memory of the last element into element
   *
   * return: 0 on success, -1 on  failure
  */
  int (*pop)(struct vector* v, void* element); 

  /*
   * length
   *
   * param: vec this
   * return: number of elements
  */
  int (*len)(struct vector*); 

  /*
   * deallocate this vector
   *
   * NOTE that is does not deallocate any internal pointers
   *
   * param: vec pointer to this
  */
  void (*dealloc) (struct vector**);

  /*
   * pop the last element off the vector
   *
   * param: vec this
   *
   * param: index
   *
   * param return by assignment: elem copies the memory of the element at index 
   *                             into `elem`
   *
   * return: 0 on success, -1 on  failure
  */
  int (*get_at)(struct vector* v, size_t index, void* elem);

  /*
   * pop the last element off the vector
   *
   * param: vec this
   *
   * param: elem search for elem by comparing 
   *        the **in memory representation of items with memcmp**
   *
   * return: index of item on success, -1 on  failure
  */
  int (*find)(struct vector*, void*);

} vector;

/*
* Create a vector with elements of some size
*
* param: with_size size of element
*
*
* returns: pointer to allocated vector
*
*/
vector* new_vector(size_t with_size);

