/*************************
        Heap Tree
      Header File
    Author : OL80
      Date : 12/01/2020
*************************/

#ifndef HEAP_H
#define HEAP_H

#include <stddef.h> /* size_t */

typedef struct heap heap_t;
typedef int (*cmp_func_t)(const void* new_data, const void* data, void *param);
typedef int (*match_func_t)(const void *data, const void *data_to_find);

extern int errno;

/* - returns a pointer to a new heap --------------------------------------- */
/* - cmp_func must be a valid function that can compare heap intended data - */
/* - param is a param to be passed to cmp_func ----------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
heap_t *HeapCreate(cmp_func_t cmp_func, void *param);

/* - destroys an existing heap --------------------------------------------- */
/* - heap must not be NULL ------------------------------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
void HeapDestroy(heap_t *heap);

/* - removes data from the tree and rebalances it -------------------------- */
/* - heap must not be NULL ------------------------------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
void *HeapRemove(heap_t *heap, match_func_t is_matched, const void *data);

/* - returns the number of elements currently in heap ---------------------- */
/* - heap must not be NULL ------------------------------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
size_t HeapSize(const heap_t *heap);

/* - returns 1 if heap is empty and 0 if not ------------------------------- */
/* - heap must not be NULL ------------------------------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
int HeapIsEmpty(const heap_t *heap);

/* - returns the highest priority element data of the heap ----------------- */
/* - heap must not be NULL ------------------------------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
void *HeapPeek(const heap_t *heap);

/* - inserts data as a new element into heap and rebalances the tree ------- */
/* - heap must not be NULL ------------------------------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
int HeapPush(heap_t *heap, void *data);

/* - returns the highest priority element data of the heap and removes it -- */
/* - heap must not be NULL ------------------------------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
void *HeapPop(heap_t *heap);

#endif /* HEAP_H */

/*
struct heap
{
	d_vector_t *dvec;
	cmp_func_t cmp_func;
	void *param;
};

*/




