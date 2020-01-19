/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Gidron Bloch ||| Date 27.11.19  */
/***********************************************************************/
/*      This file contains all the functions of the header slist.h     */
/***********************************************************************/

#include <stdio.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "slist.h"	/* function declarations */

slist_node_t *SlistCreateNode(const void *data, slist_node_t *next)
{
	slist_node_t *node = NULL;
	
	assert(NULL != data);

	node = (slist_node_t*)malloc(sizeof(slist_node_t));
	if (NULL == node)
	{
		return NULL;
	}

	node -> data = (void*)data;
	node -> next = next;

	return node;
}

void SlistInsertAfter(slist_node_t *where, slist_node_t *new_node)
{
	assert(NULL != where);
	assert(NULL != new_node);

	new_node -> next = where -> next;
	where -> next = new_node;
}

void SlistInsert(slist_node_t *where, slist_node_t *new_node)
{
	void *temp_data = NULL;

	assert(NULL != where);
	assert(NULL != new_node);

	temp_data = where -> data;
	where -> data = new_node -> data;
	new_node -> data = temp_data;

	SlistInsertAfter(where, new_node);
}

void SlistFreeAll(slist_node_t *head)
{
	slist_node_t *address_to_remove = NULL;	
	slist_node_t *next_address = NULL;
	
	assert(NULL != head);	

	next_address = head -> next;
	free(head);
	
	while (NULL != next_address)
	{	
		address_to_remove = next_address;	
		next_address = next_address -> next;				
		free(address_to_remove);
	}
}

slist_node_t *SlistRemove(slist_node_t *node)
{
	void *temp_data = node -> data;

	assert(NULL != node);	
	assert(NULL != node -> next);

	node -> data = (node -> next) -> data;
	(node -> next) -> data = temp_data;

	return SlistRemoveAfter(node);
}

slist_node_t *SlistRemoveAfter(slist_node_t *node)
{
	slist_node_t *adress_to_remove = NULL;

	assert(NULL != node);	
	
	if (NULL == node -> next)
	{
		return NULL;
	}

	adress_to_remove = node -> next;
	node -> next = (node -> next) -> next;
	
	return adress_to_remove;
}

size_t SlistCount(const slist_node_t *head)
{
	slist_node_t *node_runner = NULL;	
	size_t node_counter = 1;

	assert(NULL != head);

	node_runner = head -> next;
	
	while (NULL != node_runner)
	{
		node_runner = node_runner -> next;
		++node_counter;
	}

	return node_counter;
}

slist_node_t *SlistFlip(slist_node_t *head)
{
	slist_node_t *previous_address = NULL;
	slist_node_t *current_address = NULL;
	slist_node_t *next_address = NULL;

	assert(NULL != head);

	previous_address = head;	
	current_address = head -> next;

	while (NULL != current_address)
	{
		next_address = current_address -> next;
		current_address -> next = previous_address;
		previous_address = current_address;		
		current_address = next_address;
	}

	head -> next = NULL;

	return previous_address;
}

int SlistHasLoop(const slist_node_t *node)
{
	const slist_node_t *fast_runner = NULL;
	const slist_node_t *slow_runner = NULL;

	assert(NULL != node);

	fast_runner = node -> next;
	slow_runner = node;

	while ((NULL != fast_runner -> next) && (NULL != (fast_runner -> next) -> next))
	{
		if (fast_runner == slow_runner)
		{
			return 1;
		}

		slow_runner  = slow_runner -> next;
		fast_runner = (fast_runner -> next) -> next;
	}

	return 0;
}

void SlistForEach(slist_node_t *node, action_func_t action, void *param)
{
	slist_node_t *node_runner = NULL;
	
	assert(NULL != node);

	node_runner = node;

	while (NULL != node_runner)	
	{
		action(node_runner -> data, param);
		node_runner = node_runner -> next;
	}
}

const slist_node_t *SlistFind(const slist_node_t *head, match_func_t find, void *param)
{
	const slist_node_t *node_runner = NULL;
	
	assert(NULL != head);

	node_runner = head;

	while (NULL != node_runner)	
	{
		if (find(node_runner -> data, param)) /* if true */
		{
			return node_runner;
		}

		node_runner = node_runner -> next;
	}
	
	return NULL;
}

const slist_node_t *SlistFindIntersection(const slist_node_t *head_1, const slist_node_t *head_2)
{
	const slist_node_t *runner1 = NULL;
	const slist_node_t *runner2 = NULL;
	size_t count1 = 0, count2 = 0, difference = 0;

	assert(NULL != head_1);	
	assert(NULL != head_2);	

	runner1 = head_1;
	runner2 = head_2;
	count1 = SlistCount(head_1);
	count2 = SlistCount(head_2);

	if (count1 > count2)
	{
		difference = count1 - count2;

		while (0 < difference)
		{			
			runner1 = runner1 -> next;
			--difference;
		}
	}

	else
	{
		difference = count2 - count1;

		while (0 < difference)
		{						
			runner2 = runner2 -> next;
			--difference;
		}
	}

	while (NULL != (runner1 -> next))
	{
		if ((runner1 -> next) == (runner2 -> next))
		{
			return (runner1 -> next);
		}

		runner1 = runner1 -> next;
		runner2 = runner2 -> next;		
	}

	return NULL;
}
