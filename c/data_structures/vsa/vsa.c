/***********************************************************************/
/*  Author: Eli Umansky ||| Reviewer: Greg Markovsky ||| Date 22.12.19 */
/***********************************************************************/
/*       This file contains all the functions of the header vsa.h      */
/***********************************************************************/

#include <assert.h> /* assert */
#include <sys/types.h> /* ssize_t */
#include <stdlib.h> /* abs */

#include "vsa.h" /* vsa_t */

#define WORD_SIZE ((ssize_t)sizeof(size_t))
#define MAGIC_NUMBER (0x8080808080808080)
#define END_INDICATOR (0xBEEFFACEBEEFFACE)
#define CURRENT_NUMBER (((blk_header_t*)runner)->b_size)
#define FRONT_NUMBER (((blk_header_t*)front_runner)->b_size)
#define BACK_NUMBER (((blk_header_t*)back_runner)->b_size)
#define HEADER_SIZE ((ssize_t)sizeof(blk_header_t))
#define MAX_OF(x, y) (((x) > (y)) ? (x) : (y))

typedef struct blk_header
{
	#ifndef NDEBUG
	size_t magic_number;
	#endif
	ssize_t b_size;
} blk_header_t;

/* ---------------------------------------------------- Static Functions -----*/
static vsa_t *JumpToNextFragment(vsa_t *runner);
static vsa_t *DefragAndCountFree(vsa_t *front_runner, vsa_t *back_runner);
static vsa_t *SkipNegatives(vsa_t *front_runner);
static vsa_t *FindEnoughSpace(vsa_t *front_runner, vsa_t *back_runner,
							  size_t requested_size);
vsa_t *AllocateAndSetHeader(vsa_t *back_runner, vsa_t *front_runner,
							size_t space_to_allocate, size_t block_size);
vsa_t *AllocateExactSpace(vsa_t *back_runner, vsa_t *front_runner,
				   size_t space_to_allocate);
/* ------------------------------------------------------------- VSAInit -----*/

vsa_t *VSAInit(void *pool, size_t pool_size)
{
	size_t actual_pool_size = 0;
	vsa_t *vsa = NULL;

	assert(NULL != pool);

	vsa = pool;
	actual_pool_size = pool_size - (pool_size % WORD_SIZE) - HEADER_SIZE - WORD_SIZE;

	#ifndef NDEBUG
	((blk_header_t*)pool)->magic_number = 0;
	#endif
	((blk_header_t*)pool)->b_size = actual_pool_size;
	*(size_t*)((char*)pool + HEADER_SIZE + actual_pool_size) = END_INDICATOR;

	return vsa;
}
/* ----------------------------------------------------- VSALargestBlock -----*/
size_t VSALargestBlock(vsa_t *vsa)
{
	size_t largest_block = 0;
	size_t temp_largest_block = 0;
	vsa_t *front_runner = NULL;
	vsa_t *back_runner = NULL;

	assert(NULL != vsa);

	back_runner = vsa;
	front_runner = vsa;

	while (END_INDICATOR != *(size_t*)front_runner)
	{
		if (0 <= FRONT_NUMBER)
		{
			front_runner = DefragAndCountFree(front_runner, back_runner);
			temp_largest_block = BACK_NUMBER;
			largest_block = MAX_OF(temp_largest_block, largest_block);
		}
		else
		{
			front_runner = SkipNegatives(front_runner);
		}

		back_runner = front_runner;
	}

	return largest_block;
}
/* ------------------------------------------------------------ VSAAlloc -----*/
void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	vsa_t *front_runner = NULL;
	vsa_t *back_runner = NULL;
	size_t space_to_allocate = 0;

	assert(NULL != vsa);
	assert(0 < block_size);

	block_size += ((WORD_SIZE - (block_size % WORD_SIZE)) % WORD_SIZE);
	front_runner = vsa;
	back_runner = vsa;

	while (END_INDICATOR != *(size_t*)front_runner)
	{
		front_runner = SkipNegatives(front_runner);
		back_runner = front_runner;
		front_runner = FindEnoughSpace(front_runner, back_runner, block_size);
		space_to_allocate = BACK_NUMBER;

		if (block_size <= space_to_allocate)
		{
			if (block_size + HEADER_SIZE <= space_to_allocate)
			{
				return AllocateAndSetHeader(back_runner, front_runner,
 											space_to_allocate, block_size);				
			}
			else
			{
				return AllocateExactSpace(back_runner, front_runner,
										  space_to_allocate);
			}
		}
	}

	return NULL;
}
/* ------------------------------------------------------------- VSAFree -----*/
void VSAFree(void *ptr)
{
    if (NULL == ptr)
    {
        return;
    }

	ptr = (char*)ptr - HEADER_SIZE;

	#ifndef NDEBUG
	if (MAGIC_NUMBER != ((blk_header_t*)ptr)->magic_number)
    {
		return;
    }
    ((blk_header_t*)ptr)->magic_number = 0;
    #endif
	((blk_header_t*)ptr)->b_size = abs(((blk_header_t*)ptr)->b_size);
}
/* ---------------------------------------------------- Static Functions -----*/
static vsa_t *JumpToNextFragment(vsa_t *runner)
{
	return (char*)runner + (abs(CURRENT_NUMBER) + HEADER_SIZE);
}

static vsa_t *DefragAndCountFree(vsa_t *front_runner, vsa_t *back_runner)
{
	ssize_t consecutive_free_bytes = -HEADER_SIZE;	

	while (END_INDICATOR != *(size_t*)front_runner && 0 <= FRONT_NUMBER)
	{
		consecutive_free_bytes += FRONT_NUMBER + HEADER_SIZE;
		BACK_NUMBER = consecutive_free_bytes;
		front_runner = JumpToNextFragment(front_runner);
	}

	return front_runner;
}

static vsa_t *FindEnoughSpace(vsa_t *front_runner, vsa_t *back_runner,
							  size_t requested_size)
{
	ssize_t consecutive_free_bytes = -HEADER_SIZE;

	while (END_INDICATOR != *(size_t*)front_runner && 0 <= FRONT_NUMBER &&
		   ((ssize_t)requested_size > consecutive_free_bytes))
	{
		consecutive_free_bytes += FRONT_NUMBER + HEADER_SIZE;
		BACK_NUMBER = consecutive_free_bytes;
		front_runner = JumpToNextFragment(front_runner);
	}

	return front_runner;
}

static vsa_t *SkipNegatives(vsa_t *front_runner)
{
	while ((END_INDICATOR != *(size_t*)front_runner) && 0 > FRONT_NUMBER)
	{
		front_runner = JumpToNextFragment(front_runner);
	}

	return front_runner;
}

vsa_t *AllocateAndSetHeader(vsa_t *back_runner, vsa_t *front_runner,
							size_t space_to_allocate, size_t block_size)			
{
	BACK_NUMBER = -block_size;
	#ifndef NDEBUG
	((blk_header_t*)back_runner)->magic_number = MAGIC_NUMBER;
	#endif

    front_runner = (char*)back_runner + HEADER_SIZE + block_size;
    FRONT_NUMBER = space_to_allocate - block_size - HEADER_SIZE;
	#ifndef NDEBUG				
	((blk_header_t*)front_runner)->magic_number = 0;
	#endif

	return ((char*)back_runner + HEADER_SIZE);
}

vsa_t *AllocateExactSpace(vsa_t *back_runner, vsa_t *front_runner,
				   size_t space_to_allocate)
{			
	BACK_NUMBER = -space_to_allocate;
	#ifndef NDEBUG
	((blk_header_t*)back_runner)->magic_number = MAGIC_NUMBER;
	#endif
				
	return ((char*)back_runner + HEADER_SIZE);
}
