/************************
     Dynamic Vector
       Header File
   Author : OL80
     Date : 26/11/2019            
************************/

#ifndef DYNAMIC_VECTOR_H
#define DYNAMIC_VECTOR_H

typedef struct d_vector d_vector_t;

#include <stddef.h> /* size_t */

/* - creates a new dynamic array and returns a pointer to it's controller -- */
/* - each element size is element_size and initial capacity is 2 ----------- */
/* - element_size must be greater then 0 ----------------------------------- */
d_vector_t *DvecCreate(size_t element_size, size_t num_of_elements, int *arr);

/* - destroys dvec --------------------------------------------------------- */
/* - dvec must not be NULL ------------------------------------------------- */
void DvecDestroy(d_vector_t *dvec);

/* - copies element_size bytes from *data to the current end of the vector - */
/* - returns 0 is successful and -1 if memory allocation failed ------------ */
/* - dvec must not be NULL ------------------------------------------------- */
/* - data must not be NULL, and contain element_size bytes of data --------- */
int DvecPushBack(d_vector_t *dvec, const int data);

/* - removes the last element of the vector -------------------------------- */
/* - returns 0 is successful and -1 if memory allocation failed ------------ */
/* - dvec must not be NULL ------------------------------------------------- */
int DvecPopBack(d_vector_t *dvec);

/* - returns the adress on the index element in the array ------------------ */
/* - DvecPopBack might invalidate this data -------------------------------- */
/* - index must be >= 0 and < size ----------------------------------------- */
int *DvecGetItemAddress(const d_vector_t *dvec, size_t index);

/* - manually resize the capacity of the vector ---------------------------- */
/* - returns 0 is successful and -1 if memory allocation failed ------------ */
/* - dvec must not be NULL ------------------------------------------------- */
/* - size must be > 0 ------------------------------------------------------ */
/* - DvecPopBack / DvecPushBack might change the new size ------------------ */
/* - if size is smaller then vector size, data might be lost --------------- */
int DvecReserve(d_vector_t *dvec, size_t new_capacity);

/* - returns the current number of elements in vector ---------------------- */
/* - dvec must not be NULL ------------------------------------------------- */
size_t DvecSize(const d_vector_t *dvec);

/* - returns the current capacity of vector in element quantity ------------ */
/* - dvec must not be NULL ------------------------------------------------- */
size_t DvecCapacity(const d_vector_t *dvec);

#endif /* DYNAMIC_VECTOR_H */


