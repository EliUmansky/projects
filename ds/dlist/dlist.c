/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Daniel Oren ||| Date 08.12.19   */
/***********************************************************************/
/*      This file contains all the functions of the header dlist.h     */
/***********************************************************************/

#include <stdio.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "dlist.h"	/* dlist_t */

#define SUCCESS 0

typedef struct dlist_node dlist_node_t;

struct dlist_node
{
	void *data;
	struct dlist_node *prev;
	struct dlist_node *next;
};

struct dlist
{
	dlist_node_t *head;
	dlist_node_t *tail;
};

int SizeCountDList(void *data, void *param)
{
	(void)data;	
	++*(size_t*)param;

	return 0;
}

dlist_node_t *CreateNode(void *data)
{
	dlist_node_t *new_node = (dlist_node_t*)malloc(sizeof(dlist_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}

	new_node->data = data;

	return new_node;
}

dlist_t *DListCreate(void)
{
	dlist_node_t *head_dummy = NULL;
	dlist_node_t *tail_dummy = NULL;
	dlist_t *dlist = NULL;

	dlist = (dlist_t*)malloc(sizeof(dlist_t));
	if (NULL == dlist)
	{
		return NULL;
	}

	head_dummy = CreateNode((void*)DUMMY_DATA);
	if (NULL == head_dummy)
	{
		free(dlist);
	}
	tail_dummy = CreateNode((void*)DUMMY_DATA);
	if (NULL == tail_dummy)
	{
		free(dlist);
		free(head_dummy);
	}

	head_dummy->prev = NULL;
	head_dummy->next = tail_dummy;

	tail_dummy->prev = head_dummy;
	tail_dummy->next = NULL;

	dlist->head = head_dummy;
	dlist->tail = tail_dummy;

	return dlist;
}

void DListDestroy(dlist_t *dlist)
{
	dlist_iter_t run_iter = NULL;
	dlist_iter_t remove_iter = NULL;

	assert(NULL != dlist);

	run_iter = DListPrev(DListBegin(dlist));

	while (run_iter != DListEnd(dlist))
	{
		remove_iter = run_iter;
		run_iter = DListNext(run_iter);

		free(remove_iter);
	}

	free(run_iter);
	free(dlist);
}

dlist_iter_t DListBegin(const dlist_t *dlist)
{
	assert(NULL != dlist);

	return (dlist->head)->next;
}

dlist_iter_t DListEnd(const dlist_t *dlist)
{
	assert(NULL != dlist);

	return dlist->tail;
}

dlist_iter_t DListPrev(dlist_iter_t current)
{
	assert(NULL != current);	
	assert(NULL != current->prev);

	return current->prev;
}

dlist_iter_t DListNext(dlist_iter_t current)
{
	assert(NULL != current);	

	return current->next;
}

size_t DListSize(const dlist_t *dlist)
{
	size_t counter = 0;

	assert(NULL != dlist);

	DListForEach(DListBegin(dlist), DListEnd(dlist), SizeCountDList, &counter);

	return counter;
}

int DListIsEmpty(const dlist_t *dlist)
{
	assert(NULL != dlist);

	return DListIsSameIterator(DListBegin(dlist), DListEnd(dlist));
}

int DListForEach(dlist_iter_t start, dlist_iter_t end, action_func action,
				 void *param)
{
	dlist_iter_t diter = NULL;
	int ret_val = SUCCESS;

	assert(NULL != start);
	assert(NULL != end);

	for (diter = start; 0 == DListIsSameIterator(diter, end) &&
 		 SUCCESS == ret_val; diter = DListNext(diter))
	{
		ret_val = action(diter->data, param);
	}

	return ret_val;
}

int DListIsSameIterator(dlist_iter_t diter_1, dlist_iter_t diter_2)
{
	assert(NULL != diter_1);
	assert(NULL != diter_2);

	return (diter_1 == diter_2);
}

void *DListGetData(dlist_iter_t diter)
{
	assert(NULL != diter);

	return diter->data;
}

dlist_iter_t DListPushFront(dlist_t *dlist, void *data)
{
	return (DListInsertBefore(dlist, DListBegin(dlist), data));
}

dlist_iter_t DListPushBack(dlist_t *dlist, void *data)
{
	return (DListInsertBefore(dlist, DListEnd(dlist), data));
}

dlist_iter_t DListInsertBefore(dlist_t *dlist, dlist_iter_t where, void *data)
{
	dlist_node_t *new_node = NULL;
	dlist_iter_t diter = NULL;

	assert(NULL != dlist);
	assert(NULL != where);

	new_node = CreateNode(data);
	if (NULL == new_node)
	{
		return DListEnd(dlist);
	}

	new_node->next = where;
	new_node->prev = DListPrev(where);
	where->prev->next = new_node;
	where->prev = new_node;

	diter = new_node;

	return diter;
}

dlist_iter_t DListRemove(dlist_iter_t diter)
{
	dlist_iter_t return_diter = NULL;

	assert(NULL != diter);
	assert(NULL != DListPrev(diter));
	assert(NULL != DListNext(diter));

	return_diter = DListNext(diter);

	(diter->prev)->next = DListNext(diter);
	(diter->next)->prev = DListPrev(diter);

	free(diter);

	return return_diter;
}

void *DListPopFront(dlist_t *dlist)
{
	void *data_holder = NULL;

	assert(NULL != dlist);

	data_holder = DListGetData(DListBegin(dlist));

	DListRemove(DListBegin(dlist));

	return data_holder;
}

void *DListPopBack(dlist_t *dlist)
{
	void *data_holder = NULL;

	assert(NULL != dlist);

	data_holder = DListGetData(DListPrev(DListEnd(dlist)));

	DListRemove(DListPrev(DListEnd(dlist)));

	return data_holder;
}

dlist_iter_t DListFind(dlist_iter_t start, dlist_iter_t end,
					   match_func is_match, const void *param)
{
	dlist_iter_t diter = NULL;
	int ret_val = 0;

	assert(NULL != start);
	assert(NULL != end);

	for (diter = start; 0 == DListIsSameIterator(diter, end) &&
 		 1 != ret_val; diter = DListNext(diter))
	{
		ret_val = is_match(diter->data, param);
	}

	return (ret_val) ? DListPrev(diter) : diter;
}

dlist_iter_t DListSplice(dlist_iter_t where, dlist_iter_t first,
						 dlist_iter_t last)
{

    if (NULL == DListPrev(first) || NULL == DListPrev(where) ||
        NULL == DListNext(last))
    {
        return NULL;
    }

	(first->prev)->next = last->next;
	(last->next)->prev = first->prev;
    (where->prev)->next = first;
    first->prev = where->prev;
    where->prev = last;
    last->next = where;

	return where;
}
