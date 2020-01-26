/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Max Zazumin ||| Date 17.12.19   */
/***********************************************************************/
/*       This file contains all the functions of the header fsa.h      */
/***********************************************************************/

#include <assert.h> /* assert */

#include "fsa.h" /* fsa_t */

#define TOTAL_BLOCK_SIZE ((fsa->block_size) + (sizeof(block_header_t)))
#define CURRENT_FREE (fsa->next_free)
#define B_HEAD_SIZE (sizeof(block_header_t))

struct fsa
{
	size_t block_size;
	size_t num_of_blocks;
	size_t next_free;
};

typedef struct
{
	size_t header_offset;
} block_header_t;

size_t FSASuggestedSize(size_t block_size, size_t num_of_blocks)
{
	size_t fsa_size = sizeof(fsa_t);

	block_size = (sizeof(size_t) > block_size) ? sizeof(size_t) : block_size;
	
	return (num_of_blocks * (block_size + B_HEAD_SIZE)) + fsa_size;
}

fsa_t *FSAInit(void *pool, size_t block_size, size_t num_of_blocks)
{	
	fsa_t *fsa = NULL;	
	block_header_t *block_runner = NULL;	
	size_t *next_address_runner = NULL;
	size_t i = 0;
	
	assert(NULL != pool);

	fsa = (fsa_t*)pool;	

	fsa->block_size = (block_size < sizeof(size_t)) ? sizeof(size_t) : block_size;
	fsa->num_of_blocks = num_of_blocks;
	fsa->next_free = sizeof(fsa_t) + B_HEAD_SIZE;

	next_address_runner = (size_t*)((char*)fsa + CURRENT_FREE);
	block_runner = (block_header_t*)((char*)next_address_runner - B_HEAD_SIZE);

	for (i = 0; i < (num_of_blocks - 1); ++i)
	{
		block_runner->header_offset = sizeof(fsa_t) + (i * TOTAL_BLOCK_SIZE);	
		*next_address_runner =
		(block_runner->header_offset) + TOTAL_BLOCK_SIZE + B_HEAD_SIZE;

		block_runner = (block_header_t*)((char*)block_runner + TOTAL_BLOCK_SIZE);
		next_address_runner = 
		(size_t*)((char*)next_address_runner + TOTAL_BLOCK_SIZE);
	}
		
	block_runner->header_offset = CURRENT_FREE - B_HEAD_SIZE + (i * TOTAL_BLOCK_SIZE);
	*next_address_runner = 0;

	return fsa;
}

void *FSAAlloc(fsa_t *fsa)
{
	size_t *return_address = NULL;	

	assert(NULL != fsa);
	
	if (0 == CURRENT_FREE)
	{
		return NULL;
	}

	return_address = (size_t*)((char*)fsa + CURRENT_FREE);

	CURRENT_FREE = *return_address;

	return return_address;
}

size_t FSACountFree(const fsa_t *fsa)
{
	size_t counter = 0;
	size_t *count_runner = NULL;
	size_t offset_to_move = 0;

	assert(NULL != fsa);

	if (0 == CURRENT_FREE)
	{		
		return 0;
	}

	count_runner = (size_t*)((char*)fsa + CURRENT_FREE);
	counter = 1;

	while (0 != *count_runner)
	{		
		offset_to_move = *count_runner;
		count_runner = (size_t*)((char*)fsa + offset_to_move);
		++counter;
	}

	return counter;
}

void FSAFree(void *block)
{
	fsa_t *fsa = NULL;
	block_header_t *freed_header = NULL;	
	size_t *freed_next = NULL;
	size_t freed_offset = 0;

	if (NULL == block)
	{
		return;
	}

	freed_next = (size_t*)block; 
	freed_header = (block_header_t*)((char*)block - B_HEAD_SIZE);
	freed_offset = freed_header->header_offset;
	fsa = (fsa_t*)((char*)freed_header - freed_offset);

	*freed_next = CURRENT_FREE;
	CURRENT_FREE = freed_offset + B_HEAD_SIZE;
}
