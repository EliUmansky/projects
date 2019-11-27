/***********************************************************************/
/*              Author: Eli Umansky   Date 21.11.19                    */
/***********************************************************************/
/*      This file contains all the functions of the header stack.h     */
/***********************************************************************/

#include <stdio.h> /* size_t */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */
#include "stack.h"

struct stackstruct
{
	size_t element_size;
	char *head;
	void *end;
	void *current;
};

stack_t *StackCreate(size_t num_of_elements, size_t element_size)
{
	stack_t *stack = NULL;	
	void *arr = NULL;	
	size_t arrsize = num_of_elements * element_size;
	
	assert (0 < num_of_elements);
	assert (0 < element_size);

	stack = (stack_t*)malloc(sizeof(stack_t));

	if (NULL == stack)
	{
		return NULL;
	}

	arr = (void*)malloc(arrsize);

	if (NULL == arr)
	{
		free(stack);		

		return NULL;
	}

	stack -> element_size = element_size;
	stack -> head = arr;
	stack -> end = (char*)arr + arrsize - 1;
	stack -> current = arr;

	return stack;
}

void StackDestroy(stack_t *stack)
{
	assert (NULL != stack);

	free(stack -> head);
	free(stack);
}

size_t StackSize(const stack_t *stack)
{
	assert (NULL != stack);

	return (((size_t)stack -> current - (size_t)stack ->head)) / 
			  stack -> element_size;
}

void StackPush(stack_t *stack, const void *data)
{
	assert (NULL != stack);
	assert (NULL != data);
	assert (stack -> end >= stack -> current);


	memcpy(stack -> current, data, stack -> element_size);
	stack -> current = (char*)stack -> current + stack -> element_size; 
}

void StackPop(stack_t *stack)
{
	assert (NULL != stack);
	assert((char*)stack -> current > stack -> head);

	stack -> current = (char*)(stack -> current) - stack -> element_size; 	
}

void *StackPeek(const stack_t *stack)
{
	assert (NULL != stack);
	assert ((size_t)stack -> current >= (size_t)stack -> head);

	return ((char*)(stack -> current) - stack -> element_size); 
}

int StackIsEmpty(const stack_t *stack)
{
	assert (NULL != stack);

	return (((size_t)stack -> current) == ((size_t)stack -> head)) ? 1 : 0;
}
