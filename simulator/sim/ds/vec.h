#include <stdlib.h>
#ifndef DS_VEC
#define DS_VEC

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
  void *arr;
  size_t capacity;
  size_t size;
  size_t elem_size;

  /*
   * Internally used compare function for finding an element
   */
  int (*compare)(void *, void *);

  /*
   * Push an element onto the back of the vector
   *
   * param: vec this
   *
   * param: elem element to push
   *
   */
  void (*push)(struct vector *vec, void *elem);

  /*
   * pop the last element off the vector
   *
   * param: vec this
   *
   * param return by assignment: copies the memory of the last element into
   * element
   *
   * return: 0 on success, -1 on  failure
   */
  int (*pop)(struct vector *v, void *element);

  /*
   * length
   *
   * param: vec this
   *
   * return: number of elements
   */
  int (*len)(struct vector *);

  /*
   * deallocate this vector
   *
   * NOTE that is does not deallocate any internal pointers
   *
   * param: vec pointer to this
   */
  void (*dealloc)(struct vector **);

  /*
   * get an element from this vector
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
  int (*get_at)(struct vector *v, size_t index, void *elem);

  /*
   * get a mutable reference to an element in this vector
   *
   * param: vec this
   *
   * param: index
   *
   * param return by assignment: copies a pointer to to requested `element`
   *                             into `ptr`
   *
   * NOTE: The returned reference is only valid for as long as the immutable
   * vector reference would be valid. i.e. if the vector is mutated via push or
   * otherwise, the pointer reference is no longer guarenteed to be valid
   *
   * return: 0 on success, -1 on  failure
   */
  int (*get_mut_at)(struct vector *v, size_t index, void **ptr);

  /*
   * find a given element in vector
   *
   * equality function can be customized with `set_comparator`
   *
   * param: vec this
   *
   * param: elem search for elem by comparing
   *        the **in memory representation of items with memcmp**
   *        or a custom parameter with `set_comparator`
   *
   * return: index of item on success, -1 on  failure
   */
  int (*find)(struct vector *, void *);

  /*
   * Set the compare function to be used when finding an element.
   *
   * the function signature of compare should be:
   *
   *
   *  `int f(void* a, void* b);`
   *
   *
   * where `f` returns a number less than, equal to, or greater than zero
   * when `a` is less than, equal to, or greater than `b`, respectively
   *
   * param: vec this
   *
   * param: compare a function as described above
   */
  void (*set_comparator)(struct vector *, int (*compare)(void *, void *));

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
vector *new_vector(size_t with_size);

#endif // !DS_VEC
