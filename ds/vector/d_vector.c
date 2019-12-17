/***************************
     Dynamic Vector
      Source File
   Author : Greg Markovsky
 Reviewer : Eli Umansky
     Date : 26/11/2019            
***************************/

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "vector.h" /* d_vector_t */

#define MAX_OF(x,y) (x > y ? x : y)
#define MIN_START_CAPACITY (8) 

typedef char byte;

struct d_vector
{
	size_t element_size;
	size_t capacity;
	size_t size;
	void *start;
};

static int DvecRealloc(d_vector_t *dvec, size_t new_cap);

/* --------------------------------------------------------- Dvec Create --- */

d_vector_t *DvecCreate(size_t element_size, size_t capacity)
{
	d_vector_t *dvec = NULL;
	size_t max_capacity = 0;

	assert(0 < element_size);

	dvec = (d_vector_t *)malloc(sizeof(d_vector_t));
	if (NULL == dvec)
	{
		return NULL;
	}

	max_capacity = MAX_OF(capacity, MIN_START_CAPACITY);

	dvec -> start = malloc(max_capacity * element_size);
	if (NULL == dvec -> start)
	{
		free(dvec);
		return NULL;
	}

	dvec -> element_size = element_size;
	dvec -> capacity = max_capacity;
	dvec -> size = 0;

	return dvec;
}

/* -------------------------------------------------------- Dvec Destroy --- */

void DvecDestroy(d_vector_t *dvec)
{
	assert(NULL != dvec);

	free(dvec -> start);
	free(dvec);
}

/* ------------------------------------------------------ Dvec Push Back --- */

int DvecPushBack(d_vector_t *dvec, const void *data)
{
	byte *dest = NULL;

	assert(NULL != dvec);
	assert(NULL != data);

	dest = (byte *)(dvec -> start) + ((dvec -> size) * (dvec -> element_size));
	memcpy(dest, data, (dvec -> element_size));
	++(dvec -> size);

	if ((dvec -> size) >= (dvec -> capacity))
	{
		return DvecRealloc(dvec, ((dvec -> capacity) * 2));
	}

	return 0;
}

/* ------------------------------------------------------- Dvec Pop Back --- */

int DvecPopBack(d_vector_t *dvec)
{
	assert(NULL != dvec);
	assert(0 < (dvec ->  size));

	--(dvec -> size);

	if ((dvec -> size) < ((dvec -> capacity) / 4))
	{
		return DvecRealloc(dvec, ((dvec -> capacity) / 2));
	}
	
	return 0;
}

/* ----------------------------------------------- Dvec Get Item Address --- */

void *DvecGetItemAddress(const d_vector_t *dvec, size_t index)
{
	assert(NULL != dvec);
	assert((dvec -> size) > index);

	return (void *)((byte *)(dvec -> start) + ((dvec -> element_size) * index));
}

/* -------------------------------------------------------- Dvec Reserve --- */

int DvecReserve(d_vector_t *dvec, size_t reserve)
{
	size_t new_cap = 0;

	assert (NULL != dvec);
	assert (0 < reserve);

	new_cap = (((dvec -> size) >= reserve) ? ((dvec -> size) + 1) : reserve);

	return DvecRealloc(dvec, new_cap);
}

/* ----------------------------------------------------------- Dvec Size --- */

size_t DvecSize(const d_vector_t *dvec)
{
	assert(NULL != dvec);

	return (dvec -> size);
}

/* ------------------------------------------------------- Dvec Capacity --- */

size_t DvecCapacity(const d_vector_t *dvec)
{
	assert(NULL != dvec);

	return (dvec -> capacity);
}

/* -------------------------------------------------------- Dvec Realloc --- */

static int DvecRealloc(d_vector_t *dvec, size_t new_cap)
{
		void *start_new = NULL;

		assert(NULL != dvec);
		assert(0 < new_cap);

		start_new = realloc((dvec -> start), new_cap * (dvec -> element_size));
		if (NULL == start_new)
		{
			return -1;
		}
		dvec -> capacity = new_cap;
		dvec -> start = start_new;

		return 0;
}




