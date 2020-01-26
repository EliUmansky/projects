/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Jenny Kogan ||| Date 26.11.19   */
/***********************************************************************/
/*      This file contains all the functions of the header dvec.h      */
/***********************************************************************/

#include <stdio.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "vector.h"	/* function declarations */

#define MIN_CAPACITY (16)
#define DOUBLE_CAPACITY ((dvec->capacity) * 2)
#define QUARTER_CAPACITY ((dvec->capacity) / 4)
#define HALF_CAPACITY ((dvec->capacity) / 2)

struct d_vector
{
	size_t element_size;
	void *start;
	size_t size;
	size_t capacity;
};

d_vector_t *DvecCreate(size_t element_size, size_t num_of_elements, int *arr)
{
	d_vector_t *dvec = NULL;

	assert(0 < element_size);
	assert(0 < num_of_elements);

	dvec = (d_vector_t*)malloc(sizeof(d_vector_t));
	if (NULL == dvec)
	{
		return NULL;
	}

	if (8 > num_of_elements)
	{
		num_of_elements = 8;
	}

	arr = arr;
	if (NULL == arr)
	{
		free(dvec);		

		return NULL;
	}

	dvec->element_size = element_size;
	dvec->start = arr;
	dvec->size = 0;
	dvec->capacity = num_of_elements;

	return dvec;
}

void DvecDestroy(d_vector_t *dvec)
{
	assert(NULL != dvec);	

	free(dvec->start);
	free(dvec);
}

int DvecPushBack(d_vector_t *dvec, int data)
{
	void *temp_start = NULL;	

	assert(NULL != dvec);
	assert((dvec->capacity) > (dvec->size));

	memcpy((char*)(dvec->start) + (dvec->size * dvec->element_size),
 			&data, dvec->element_size);
	++(dvec->size);

	if ((dvec->capacity) == (dvec->size))
	{
		temp_start = realloc(dvec->start, DOUBLE_CAPACITY * 
							 (dvec->element_size));
		if (NULL == temp_start)
		{
			return -1;
		}
		
		dvec->start = temp_start;
		(dvec->capacity) = DOUBLE_CAPACITY;
	}	
	
	return 0;
}

int DvecPopBack(d_vector_t *dvec)
{
	void *temp_start = NULL;	

	assert(NULL != dvec);
	assert(0 < (dvec->size));

	--(dvec->size);

	if (QUARTER_CAPACITY > (dvec->size))
	{
		temp_start = realloc(dvec->start, HALF_CAPACITY *
							 (dvec->element_size));
		if (NULL == (dvec->start))
		{
			return -1;
		}
		
		dvec->start = temp_start;		
		(dvec->capacity) = HALF_CAPACITY;
	}	
	
	return 0;
}

int *DvecGetItemAddress(const d_vector_t *dvec, size_t index)
{
	assert(NULL != dvec);
	assert((dvec->size) > index);	

	return (int*)((char*)(dvec->start) + ((dvec->element_size) * index));
}

int DvecReserve(d_vector_t *dvec, size_t new_capacity)
{
	void *temp_start = NULL;	

	assert(NULL != dvec);
	assert(0 < new_capacity);
	
	if (new_capacity > (dvec->size))
	{
		temp_start = realloc(dvec->start, (dvec->element_size) * 
							 new_capacity);
		if (NULL == dvec)
		{
			return -1;
		}
		
		dvec->start = temp_start;	
		dvec->capacity = new_capacity;
	}

	else
	{
		temp_start = realloc(dvec->start, ((dvec->element_size) * 
								(dvec->size) + 1));
		if (NULL == dvec)
		{
			return -1;
		}
		
		dvec->start = temp_start;	
		dvec->capacity = dvec->size + 1;
	}

	return 0;
}

size_t DvecSize(const d_vector_t *dvec)
{
	assert(NULL != dvec);

	return (dvec->size);
}

size_t DvecCapacity(const d_vector_t *dvec)
{
	assert(NULL != dvec);

	return (dvec->capacity);
}

