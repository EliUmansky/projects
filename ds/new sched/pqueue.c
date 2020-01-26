/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Ben Shomroni ||| Date 10.12.19  */
/***********************************************************************/
/*     This file contains all the functions of the header pqueue.h     */
/***********************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "pqueue.h" /* p_queue_t */

#define HEAP (pqueue->heap)

typedef struct wrapper
{
	compare_func compare;
	void *param;
} wrapper_t;

struct p_queue
{
	heap_t *heap;
	wrapper_t *wrapper;
};

static int CmpToBool(const void *new_data, const void *data, void *func_param);

p_queue_t *PQCreate(compare_func compare, void *param)
{
	p_queue_t *pqueue = {0};

	assert(NULL != compare);

	pqueue = (p_queue_t*)malloc(sizeof(p_queue_t));
	if (NULL == pqueue)
	{
		return NULL;
	}

	pqueue->wrapper = (wrapper_t*)malloc(sizeof(wrapper_t));
	if (NULL == pqueue->wrapper)
	{
		free(pqueue);

		return NULL;
	}

	pqueue->wrapper->compare = compare;
	pqueue->wrapper->param = param;

	pqueue->heap = HeapCreate(compare, param);
	if (NULL == pqueue->heap)
	{
		free(pqueue->wrapper);
		free(pqueue);

		return NULL;
	}

	return pqueue;
}

void PQDestroy(p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	HeapDestroy(HEAP);
	free(pqueue->wrapper);
	free(pqueue);
}

int PQIsEmpty(const p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	return HeapIsEmpty(HEAP);
}

size_t PQSize(const p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	return HeapSize(HEAP);
}

int PQEnqueue(p_queue_t *pqueue, void *data)
{
	assert(NULL != pqueue);
	assert(NULL != data);

	return HeapPush(HEAP, data);
}

void *PQDequeue(p_queue_t *pqueue)
{
	assert(NULL != pqueue);
	assert(!PQIsEmpty(pqueue));

	return HeapPop(HEAP);
}

void *PQPeek(const p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	return HeapPeek(HEAP);
}

void PQClear(p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	while(!PQIsEmpty(pqueue))
	{
		PQDequeue(pqueue);
	}
}

void *PQErase(p_queue_t *pqueue, is_matched_func is_match, void *data_to_find)
{
	assert(NULL != pqueue);

	return HeapRemove(HEAP, is_match, data_to_find);
}

static int CmpToBool(const void *new_data, const void *data, void *param)
{
	int result = ((wrapper_t*)param)->compare(new_data, data,
											((wrapper_t*)param)->param);

	return (result < 0);
}

