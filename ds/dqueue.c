/*******************************************************************************
*								 Name: Daniel oren*
* 								 Date: 2. 12. 19*
* 								        queue
*******************************************************************************/
#include <stdio.h> /*size_t*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/

#include "slist.h" /*slist function*/	
#include "queue.h" /*slist function*/
	
/******************************************************************************/
struct queue 
{
	slist_node_t *head;
	slist_node_t *tail;
};
/******************************************************************************/
queue_t *QueCreate(void)
{
	queue_t *new_q = NULL;
	slist_node_t *dummy = NULL;
	size_t dummy_data = -1;
	
	new_q = (queue_t *)malloc(sizeof(queue_t));
	if(NULL == new_q)
	{
		return NULL;
	}

	dummy = SlistCreateNode(&dummy_data, NULL);
	if(NULL == dummy)
	{	
		free(new_q);
		return NULL;
	}
	new_q->head = dummy;
	new_q->tail = dummy;

	return new_q;	
}
/******************************************************************************/
void QueDestroy(queue_t *queue)
{
	assert(NULL != queue);

	SlistFreeAll(queue->head);
	free(queue);
		
}
/******************************************************************************/
int QueEnqueue(queue_t *queue, const void *data)
{
	slist_node_t *node_to_insert = NULL;

	assert(NULL != queue);
	assert(NULL != data);

	node_to_insert = SlistCreateNode(data, NULL);
	if(NULL == node_to_insert)
	{
		return -1;
	}
	SlistInsert(queue->tail, node_to_insert);
	queue->tail = node_to_insert;
	
	return 0;
}
/******************************************************************************/
void QueDequeue(queue_t *queue)
{
	slist_node_t *holder = NULL;

	assert(NULL != queue);
	assert(!QueIsEmpty(queue));
	
	holder = queue->head;
	queue->head = (queue->head)-> next;
	free(holder);
}
/******************************************************************************/
void *QuePeek(const queue_t *queue)
{
	assert(NULL != queue);
	assert(!QueIsEmpty(queue));

	return ((queue->head)->data);
}
/******************************************************************************/
int QueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	return (queue->head == queue->tail);
}
/******************************************************************************/
size_t QueSize(const queue_t *queue)
{
	assert(NULL != queue);
	return (SlistCount(queue->head) - 1);
}
/******************************************************************************/ 
queue_t *QueAppend(queue_t *dest, queue_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);

	if(!QueIsEmpty(src))
	{		
		dest->tail->next = src->head;
		free(SlistRemove(dest->tail));
		dest->tail = src->tail;
	}

	free(src);

	return dest;
}
/******************************************************************************/
