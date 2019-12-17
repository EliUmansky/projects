/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Ben Shomroni ||| Date 10.12.19  */
/***********************************************************************/
/*     This file contains all the functions of the header pqueue.h     */
/***********************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "pqueue.h" /* p_queue_t */

#define QLIST (pqueue->qlist)

typedef struct wrapper
{
	compare_func compare;
	void *param;
} wrapper_t;

struct p_queue
{
	sorted_list_t *qlist;
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

	pqueue->qlist = SortedListCreate(&CmpToBool, pqueue->wrapper);
	if (NULL == pqueue->qlist)
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

	SortedListDestroy(QLIST);
	free(pqueue->wrapper);
	free(pqueue);
}

int PQIsEmpty(const p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	return SortedListIsEmpty(QLIST);
}

size_t PQSize(const p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	return SortedListSize(QLIST);
}

int PQEnqueue(p_queue_t *pqueue, void *data)
{
	sorted_list_iter_t new_iter = {0};

	assert(NULL != pqueue);
	assert(NULL != data);

	new_iter = SortedListInsert(QLIST, data);

	return (!SortedListIsSameIter(new_iter, SortedListEnd(QLIST)));
}

void *PQDequeue(p_queue_t *pqueue)
{
	assert(NULL != pqueue);
	assert(!PQIsEmpty(pqueue));

	return SortedListPopFront(QLIST);
}

void *PQPeek(const p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	return SortedListGetData(SortedListBegin(QLIST));
}

void PQClear(p_queue_t *pqueue)
{
	assert(NULL != pqueue);

	while(!PQIsEmpty(pqueue))
	{
		PQDequeue(pqueue);
	}
}

void *PQErase(p_queue_t *pqueue, is_matched_func is_match, void *param)
{
	sorted_list_iter_t result_iter= {0};
	sorted_list_iter_t start= {0};
	sorted_list_iter_t end= {0};
	void *data = NULL;

	assert(NULL != pqueue);

	start= SortedListBegin(pqueue->qlist);
	end = SortedListEnd(pqueue->qlist);
		
	result_iter = SortedListFindIf(start, end, is_match, param);
	

	if(!SortedListIsSameIter(result_iter, end))
	{
		data = SortedListGetData(result_iter);
		SortedListRemove(result_iter);
	
	}

	return data;
}

static int CmpToBool(const void *new_data, const void *data, void *param)
{
	int result = ((wrapper_t*)param)->compare(new_data, data,
											((wrapper_t*)param)->param);

	return (result < 0);
}

