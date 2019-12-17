/***********************************************************************/
/*  Author: Eli Umansky ||| Reviewer: Greg Markovsky ||| Date 09.12.19 */
/***********************************************************************/
/*     This file contains all the functions of the header sorlist.h    */
/***********************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dlist.h" /* dlist_t */
#include "sortlist.h" /* sorted_list_iter_t */

#define DLIST (sorted_list->dlist)
#define INTERNAL_ITER (sorted_iter.internal_iter)
#define PREV (SortedListPrev)
#define NEXT (SortedListNext)
#define BEGIN (SortedListBegin)
#define END (SortedListEnd)
#define GET_DATA (SortedListGetData)

struct sorted_list
{
	dlist_t *dlist;
	is_before_func is_before;
	void *param;
};

static sorted_list_iter_t FindWhere(sorted_list_t *sorted_list, 
			sorted_list_iter_t start, sorted_list_iter_t end, const void *data);

sorted_list_t *SortedListCreate(is_before_func is_before, void *param)
{
	sorted_list_t *sorted_list = NULL;	
	dlist_t *dlist = NULL;

	dlist = DListCreate();
	if (NULL == dlist)
	{
		return NULL;
	}

	sorted_list = (sorted_list_t*)malloc(sizeof(sorted_list_t));
	if (NULL == sorted_list)
	{
		free(dlist);

		return NULL;
	}
	
	sorted_list->dlist = dlist;
	sorted_list->is_before = is_before;
	sorted_list->param = param;

	return sorted_list;
}

void SortedListDestroy(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	DListDestroy(DLIST);	
	free(sorted_list);
}

size_t SortedListSize(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DListSize(DLIST);
}

sorted_list_iter_t SortedListBegin(const sorted_list_t *sorted_list)
{
	sorted_list_iter_t sorted_iter = {0};

	assert(NULL != sorted_list);

	INTERNAL_ITER = DListBegin(DLIST);

	return sorted_iter;
}

sorted_list_iter_t SortedListEnd(const sorted_list_t *sorted_list)
{
	sorted_list_iter_t sorted_iter = {0};

	assert(NULL != sorted_list);

	INTERNAL_ITER = DListEnd(DLIST);

	return sorted_iter;
}

sorted_list_iter_t SortedListNext(sorted_list_iter_t curr)
{
	sorted_list_iter_t sorted_iter = {0};

	INTERNAL_ITER = DListNext(curr.internal_iter);

	return sorted_iter;
}

sorted_list_iter_t SortedListPrev(sorted_list_iter_t curr)
{
	sorted_list_iter_t sorted_iter = {0};

	INTERNAL_ITER = DListPrev(curr.internal_iter);

	return sorted_iter;
}

int SortedListIsEmpty(const sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);

	return DListIsEmpty(DLIST);
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t sorted_iter)
{
	sorted_list_iter_t sorted_iter_to_remove = {0};
	
	sorted_iter_to_remove.internal_iter = DListRemove(INTERNAL_ITER);
	
	return sorted_iter_to_remove;
}

void *SortedListPopFront(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);	
	assert(!SortedListIsEmpty(sorted_list));

	return DListPopFront(DLIST);
}

void *SortedListPopBack(sorted_list_t *sorted_list)
{
	assert(NULL != sorted_list);
	assert(!SortedListIsEmpty(sorted_list));

	return DListPopBack(DLIST);
}

int SortedListForEach(sorted_list_iter_t start, sorted_list_iter_t end, 
			     	  action_func action, void *param)
{
	assert(NULL != action);

	return DListForEach(start.internal_iter, end.internal_iter, action, param);
}

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t start,
			 sorted_list_iter_t end, match_func is_matched, const void *param)
{
	sorted_list_iter_t sorted_iter = {0};

	assert(NULL != is_matched);

	INTERNAL_ITER = DListFind(start.internal_iter, end.internal_iter,
							  is_matched, param);

	return sorted_iter;
}

void *SortedListGetData(sorted_list_iter_t sorted_iter)
{
	return DListGetData(INTERNAL_ITER);
}

int SortedListIsSameIter(sorted_list_iter_t sorted_iter_1,
						 sorted_list_iter_t sorted_iter_2)
{
	return DListIsSameIterator(sorted_iter_1.internal_iter,
							   sorted_iter_2.internal_iter);
}

sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data)
{
	sorted_list_iter_t where = {0};	
	sorted_list_iter_t new_iter = {0};

	assert(NULL != sorted_list);
	
	where = FindWhere(sorted_list, BEGIN(sorted_list), END(sorted_list), data);
	
	new_iter.internal_iter = DListInsertBefore(DLIST, where.internal_iter, data);

	return new_iter;
}

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
	sorted_list_iter_t run_dest = {0};
	sorted_list_iter_t run_src_small = {0};
	sorted_list_iter_t run_src_big = {0};

	assert(NULL != dest);
	assert(NULL != src);	

	run_dest = BEGIN(dest);
	run_src_small = BEGIN(src);
	run_src_big = BEGIN(src);

	while (0 != SortedListSize(src))
	{	
		run_dest = FindWhere(dest, run_dest, END(dest), GET_DATA(run_src_small));
		
		if (SortedListIsSameIter(run_dest, END(dest)))
		{
			DListSplice(run_dest.internal_iter, run_src_small.internal_iter, 	 
					(PREV(END(src))).internal_iter);

			break;
		}		
		
		run_src_big = FindWhere(src, run_src_small, END(src), GET_DATA(run_dest));
		
		DListSplice(run_dest.internal_iter, run_src_small.internal_iter, 	 
					PREV(run_src_big).internal_iter);

		run_src_small = run_src_big;
	}
}

sorted_list_iter_t SortedListFind(sorted_list_t *sorted_list, 
			sorted_list_iter_t start, sorted_list_iter_t end, const void *data)
{
	sorted_list_iter_t match = {0};

	match = start;

	while (!SortedListIsSameIter(match, end) &&
		   sorted_list->is_before(GET_DATA(match), data, sorted_list->param))
	{
 		match = NEXT(match);
	}
	
	if (!SortedListIsSameIter(match, END(sorted_list)) &&
		sorted_list->is_before(data, GET_DATA(match), sorted_list->param))
	{		
		return END(sorted_list);
	}
	else
	{
		return match;
	}
}	

static sorted_list_iter_t FindWhere(sorted_list_t *sorted_list, 
			sorted_list_iter_t start, sorted_list_iter_t end, const void *data)
{
	sorted_list_iter_t where = {0};	
	int ret_val = 0;

	assert(NULL != sorted_list);

	for (where.internal_iter = start.internal_iter;
		 (!SortedListIsSameIter(where, end)) && (0 == ret_val); 
		 where = NEXT(where))
	{
		ret_val = sorted_list->is_before(data, GET_DATA(where), sorted_list->param);
	}
	
	return (ret_val) ? PREV(where) : where;
}
