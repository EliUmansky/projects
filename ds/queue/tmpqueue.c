/***********************************************************************/
/*  Author: Eli Umansky || Reviewer: Igal Bogopolsky || Date 27.11.19  */
/***********************************************************************/
/*      This file contains all the functions of the header queue.h     */
/***********************************************************************/

#include <stdio.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "queue.h"	/* queue_t */
#include "slist.h"	/* slist_t */

#define DUMMY 1;
struct queue
{
	slist_node_t *head;
	slist_node_t *tail;
};

queue_t *QueCreate(void)
{	
	queue_t *queue = (queue_t*)malloc(sizeof(queue_t));
	if (NULL == queue)
	{
		return NULL;
	}

	queue->head = SlistCreateNode(&queue, NULL);
	if (NULL == queue->head)
	{
		free(queue);

		return NULL;
	}
	queue->tail = queue->head;

	return queue;
}

void QueDestroy(queue_t *queue)
{
	assert(NULL != queue);

	SlistFreeAll(queue->head);
	free(queue);
}

int QueEnqueue(queue_t *queue, const void *data)
{
	slist_node_t *new_node = NULL;

	assert(NULL != queue);

	new_node = SlistCreateNode(data, NULL);
	if (NULL == new_node)
	{
		return 1;
	}

	SlistInsert(queue->tail, new_node);

	queue->tail = new_node;

	return 0;	
}

void QueDequeue(queue_t *queue)
{
	slist_node_t *node_to_destroy = NULL;	
	
	assert(NULL != queue);
	assert(!QueIsEmpty(queue));

	node_to_destroy = queue->head;	
	queue->head = (queue->head)->next;
	free(node_to_destroy);
}

void *QuePeek(const queue_t *queue)
{
	assert(NULL != queue);
	assert(queue->head != queue->tail);

	return (queue->head)->data;
}

int QueIsEmpty(const queue_t *queue)
{
	return (queue->head == queue->tail);
}

size_t QueSize(const queue_t *queue)
{
	return SlistCount(queue->head) - DUMMY;
}

queue_t *QueAppend(queue_t *dest, queue_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);

	if (!QueIsEmpty(src))
	{		
		(dest->tail)->next = src->head;
		free(SlistRemove(dest->tail));
		dest->tail = src->tail;
	}

	free(src);

	return dest;
}

