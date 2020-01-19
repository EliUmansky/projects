/*************************
  Variable Size Allocator
      Header File
    Author : OL80
      Date : 18/12/2019         
*************************/

#ifndef VARIABLE_SIZE_ALLOCATOR_H
#define VARIABLE_SIZE_ALLOCATOR_H

#include <stddef.h> /* size_t */

typedef void vsa_t;

/* - initializes pool as VSA managed space and returns a pointer to it ----- */
/* - pool must not be null ------------------------------------------------- */
vsa_t *VSAInit(void *pool, size_t pool_size);

/* - allocates a block the size of size and returns a pointer to it -------- */
/* - if there was no block large enough, returns null ---------------------- */
/* - vsa must not be null -------------------------------------------------- */
/* - size must be greater then 0 ------------------------------------------- */
void *VSAAlloc(vsa_t *vsa, size_t block_size);

/* - frees an allocated block of data -------------------------------------- */
/* - block must be a pointer previously recived from VSAAlloc -------------- */
void VSAFree(void *ptr);

/* - returns the size of the larges block available for allocation --------- */
/* - vsa must not be null -------------------------------------------------- */
size_t VSALargestBlock(vsa_t *vsa);

#endif /* VARIABLE_SIZE_ALLOCATOR_H */
