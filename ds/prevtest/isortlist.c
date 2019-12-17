/********************************************
*		Author: Igal Bogopolsky				*
*		Reviewer: Ben Shomroni				*
*		Date: 7.12.2019						*
*	Data Structures - Doubly Linked List	*
********************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "sortlist.h" /* sorted_list_t */
#include "dlist.h" /* dlist_t */

struct sorted_list
{
	dlist_t *dlist;
	is_before_func is_before;
	void *param;
};

static sorted_list_iter_t GetWhere(sorted_list_t *sorted_list, void *data);

sorted_list_t *SortedListCreate(is_before_func is_before, void *param)
{
	sorted_list_t *new_list = (sorted_list_t *)malloc(sizeof(sorted_list_t));
	if(NULL == new_list)
	{
		return NULL;
	}

	new_list->dlist = DListCreate();
	new_list->is_before = is_before;
	new_list->param = param;	
	
	return new_list;
}

void SortedListDestroy(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	DListDestroy(sorted_list->dlist);
	free(sorted_list);
}

sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data)
{
	sorted_list_iter_t iter = {NULL};	

	assert(NULL != sorted_list);

	iter.internal_iter = DListBegin(sorted_list->dlist);

	while(DListEnd(sorted_list->dlist) != iter.internal_iter)
	{
		if(sorted_list->is_before(data, SortedListGetData(iter), sorted_list->param))
		{
			break;			
		}

		iter = SortedListNext(iter);
	}

	iter.internal_iter = DListInsertBefore(sorted_list->dlist, iter.internal_iter, data);

	return iter;
}

size_t SortedListSize(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DListSize(sorted_list->dlist);
}

sorted_list_iter_t SortedListBegin(const sorted_list_t *sorted_list)
{
	sorted_list_iter_t iter = {NULL};

	assert(NULL != sorted_list);

	iter.internal_iter = DListBegin(sorted_list->dlist);

	return iter;
}

sorted_list_iter_t SortedListEnd(const sorted_list_t *sorted_list)
{
	sorted_list_iter_t iter = {NULL};

	assert(NULL != sorted_list);

	iter.internal_iter = DListEnd(sorted_list->dlist);

	return iter;
}

sorted_list_iter_t SortedListNext(sorted_list_iter_t curr)
{
	sorted_list_iter_t iter = {NULL};

	assert(NULL != curr.internal_iter);

	iter.internal_iter = DListNext(curr.internal_iter);

	return iter;
}

sorted_list_iter_t SortedListPrev(sorted_list_iter_t curr)
{
	sorted_list_iter_t iter = {NULL};

	assert(NULL != curr.internal_iter);

	iter.internal_iter = DListPrev(curr.internal_iter);

	return iter;
}

int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DListIsEmpty(sorted_list->dlist);
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t sorted_iter)
{
	sorted_iter.internal_iter = DListRemove(sorted_iter.internal_iter);

	return sorted_iter;
}

void *SortedListPopFront(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DListPopFront(sorted_list->dlist);
}

void *SortedListPopBack(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DListPopBack(sorted_list->dlist);
}

int SortedListForEach(sorted_list_iter_t start, sorted_list_iter_t end, 
				action_func action, void *param)
{
	int status = 0;

	assert(NULL != start.internal_iter);
	assert(NULL != end.internal_iter);

	status = DListForEach(start.internal_iter, end.internal_iter, action, param);

	return status;
}

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t start,
			 sorted_list_iter_t end, match_func is_matched, const void *param)
{
	sorted_list_iter_t finder = {NULL};

	assert(NULL != start.internal_iter);
	assert(NULL != end.internal_iter);

	finder.internal_iter = DListFind(start.internal_iter, end.internal_iter,
					   is_matched, param);

	return finder;
}

void *SortedListGetData(sorted_list_iter_t sorted_iter)
{
	return DListGetData(sorted_iter.internal_iter);
}

int SortedListIsSameIter(sorted_list_iter_t sorted_iter_1,
											 sorted_list_iter_t sorted_iter_2)
{
	return (sorted_iter_1.internal_iter == sorted_iter_2.internal_iter);
}

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
	sorted_list_iter_t where = {0};
	sorted_list_iter_t first = {0};
	sorted_list_iter_t last = {0};

	assert(NULL != dest);
	assert(NULL != src);

	while(!SortedListIsEmpty(src))
	{
		first = SortedListBegin(src);
		where = GetWhere(dest, SortedListGetData(first));

		if (SortedListIsSameIter(where, SortedListEnd(dest)))
		{
			last = SortedListEnd(src);
		}
		else
		{
			last = GetWhere(src, SortedListGetData(where));
		}
		last = SortedListPrev(last);

		DListSplice(where.internal_iter, first.internal_iter, last.internal_iter);
	}
}

sorted_list_iter_t SortedListFind(sorted_list_t *sorted_list, 
			sorted_list_iter_t start, sorted_list_iter_t end, const void *data)
{
	sorted_list_iter_t runner = {0};
	is_before_func is_before = NULL;
	void *param = NULL;

	assert(NULL != sorted_list);

	runner = start;
	is_before = sorted_list->is_before;
	param = sorted_list->param;

	while(!SortedListIsSameIter(runner, end))
	{
		if((!is_before(data, SortedListGetData(runner), param)) &&
			(!is_before(SortedListGetData(runner), data, param)))
		{
			return runner;
		}
		runner = SortedListNext(runner);
	}

	return end;
}

/*------------------ Internal Functions --------------------*/

static sorted_list_iter_t GetWhere(sorted_list_t *sorted_list, void *data)
{
	sorted_list_iter_t runner = {0};
	is_before_func is_before = NULL;
	void *param = NULL;

	assert(NULL != sorted_list);

	runner = SortedListBegin(sorted_list);
	is_before = sorted_list->is_before;
	param = sorted_list->param;

	while((!SortedListIsSameIter(runner, SortedListEnd(sorted_list))) &&
		  (!is_before(data, SortedListGetData(runner), param)))
	{
		runner = SortedListNext(runner);
	}

	return runner;
}
