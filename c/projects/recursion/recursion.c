/******************************************************************************/
/*              Author: Eli Umansky || Reviewer: Andrew Galuza                */
/******************************************************************************/
/*                 Exercise: Recursion || Date: 12.01.20                      */
/******************************************************************************/

#include <string.h> /* strncmp */

#include "recursion.h" /* recursion functions */
#include "stack.h" /* stack_t */

#define MAX_OF(x, y) ((x > y) ? (x) : (y))
#define MIN_OF(x, y) ((x > y) ? (y) : (x))

static void SortElements(stack_t *stack, int num_to_cmp);

size_t Fibonacci(size_t num)
{
	size_t left_num = 1, middle_num = 1, right_num = 2, location = num;

	while (0 < location)
	{
		left_num = middle_num;
		middle_num = right_num;
		right_num = left_num + middle_num;

		--location;
	}

	return left_num;
}

size_t RecFibonacci(size_t num)
{
	if (1 == num || 0 == num)
	{
		return 1;
	}

	return RecFibonacci(num - 1) + RecFibonacci(num - 2);
}

size_t RecStrLen(const char *str)
{
	if ('\0' == *str)
	{
		return 0;
	}

	return RecStrLen(str + 1) + 1;
}

int RecStrCmp(const char *str_a, const char *str_b)
{
	if ('\0' == *str_a || *str_a != *str_b)
	{
		return *str_a - *str_b;
	}

	return RecStrCmp(str_a + 1, str_b + 1);
}

char *RecStrCpy(char *dest, const char *src)
{
	*dest = *src;

	if ('\0' == *src)
	{
		return dest;
	}

	RecStrCpy(dest + 1, src + 1);

	return dest;
}

slist_node_t *RecSlistFlip(slist_node_t *head)
{
	slist_node_t *new_head = NULL;

	if (NULL == head->next)
	{
		return head;
	}

	new_head = RecSlistFlip(head->next);
	head->next->next = head;
	head->next = NULL;

	return new_head;
}

void RecStackSort(stack_t *stack)
{
	int num_to_cmp = 0;

    num_to_cmp = *(int*)StackPeek(stack);
    StackPop(stack);

    if (1 == StackSize(stack))
    {
        SortElements(stack, num_to_cmp);
        return;
    }

    RecStackSort(stack);

    SortElements(stack, num_to_cmp);  
}

static void SortElements(stack_t *stack, int num_to_cmp)
{
    int first_in_stack = *(int*)StackPeek(stack);

	if (StackIsEmpty(stack) || first_in_stack > num_to_cmp)
	{
		StackPush(stack, &num_to_cmp);
		return;
	}
  
    StackPop(stack);
    
	SortElements(stack, num_to_cmp);
	
	StackPush(stack, &first_in_stack);
}

char *RecStrCat(char *dest, const char *src)
{
	size_t dest_length = RecStrLen(dest);

	if ('\0' == *src)
	{
		return dest;
	}

	*(dest + dest_length) = *src;
	*(dest + dest_length + 1) = '\0';

	RecStrCat(dest, src + 1);

	return dest;
}

char *RecStrStr(const char *haystack, const char *needle)
{
	char *haystack_runner = (char*)haystack;	
	size_t needle_len = RecStrLen(needle);

	if (0 == strncmp(haystack_runner, needle, needle_len))
	{
		return haystack_runner;
	}
	else if ('\0' == *haystack_runner)
	{
		return NULL;
	}
	
	haystack_runner = RecStrStr(haystack_runner + 1, needle);

	return haystack_runner;
}
