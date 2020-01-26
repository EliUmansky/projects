#include <stdio.h>
#include "stack.h"

#define RUN_TEST(test, exp) (test == exp) ? printf("OK\n") : printf("FAIL\n"); 
#define NULL_TEST(test, exp) (test != exp) ? printf("OK\n") : printf("FAIL\n"); 

int main()
{
	size_t num_of_elements = 8;
	size_t element_size = 4;
	unsigned int data[] = {1, 8, 27, 64, 125, 216, 343, 512, 729, 1000, 1331};
	size_t i = 0;
	unsigned int *peek = NULL;
	stack_t *stack = StackCreate(num_of_elements, element_size);
	
	printf("The StackCreate function must return a pointer that is not NULL: ");
	NULL_TEST(stack, NULL);

	printf("At the beginning the size of stack is 0: ");
	RUN_TEST(StackSize(stack), 0);

	while (i < 8)
	{
		StackPush(stack, (void*)(data + i));
		printf("After calling StackPush- size of stack is %ld: ", i + 1);
		RUN_TEST(StackSize(stack), i + 1);
		++i;
	}

	while (i > 4)
	{
		StackPop(stack);
		printf("After calling StackPop- size of stack is %ld: ", i - 1);
		RUN_TEST(StackSize(stack), i - 1);
		--i;
	}

	printf("After calling StackPop 4 times, size of stack shouldn't be 0: ");
	RUN_TEST(StackIsEmpty(stack), 0);

	while(i > 1)
	{
		StackPop(stack);
		printf("After popping, let's peek- last element is %ld: ",
			   (i - 1) * (i - 1) * (i - 1)); 
		peek = StackPeek(stack);
		RUN_TEST(*peek, (i - 1) * (i - 1) * (i - 1));
		printf("%u\n", *peek);
		--i;
	}
	
	
	printf("Only one element left, stack is still not empty: ");
	RUN_TEST(StackIsEmpty(stack), 0);

	StackPop(stack);
	printf("We poped the last one, now stack is empty: ");
	RUN_TEST(StackIsEmpty(stack), 1);
	
	StackDestroy(stack);
	
	return 0;
}

