/*************************
  Fixed Size Allocation
      Header File
    Author : OL80
      Date : 16/12/2019          
*************************/

#ifndef FIXED_SIZE_ALLOCATION_H
#define FIXED_SIZE_ALLOCATION_H

#include <stddef.h> /* size_t */

typedef struct fsa fsa_t;

/* - returns the correct size for the pool, based on the desired pool size - */
/* - time complexity O(1) -------------------------------------------------- */
size_t FSASuggestedSize(size_t block_size, size_t num_of_blocks);

/* - returns a pointer to a newly initialized fixed size allocatot --------- */
/* - the allocator will contain num_of_elements chunks of bytes, each ------ */
/* - the size of element_size. pool must be of FSASuggestedSize based of --- */
/* - that total. ----------------------------------------------------------- */
/* - pool must not be NULL ------------------------------------------------- */
/* - num_of_elements must be more then 0 ----------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
fsa_t *FSAInit(void *pool, size_t block_size, size_t num_of_blocks);

/* - allocates an element and returns a pointer to it ---------------------- */
/* - there must be at least one free element ------------------------------- */
/* - fsalloc must not be NULL ---------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
void *FSAAlloc(fsa_t *fsa);

/* - frees an allocated element -------------------------------------------- */
/* - element num not be NULL and must be a previously allocated pointer ---- */
/* - time complexity O(1) -------------------------------------------------- */
void FSAFree(void *block);

/* - returns the number of free elements in fsalloc ------------------------ */
/* - fsalloc must not be NULL ---------------------------------------------- */
/* - time complexity O(n) -------------------------------------------------- */
size_t FSACountFree(const fsa_t *fsa);

#endif /* FIXED_SIZE_ALLOCATION_H */
