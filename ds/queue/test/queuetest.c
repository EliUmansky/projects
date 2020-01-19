#include <stdio.h> /* size_t */
#include <stdlib.h> /* free */

#include "slist.h"
#include "queue.h"

struct queue
{
	slist_node_t *head;
	slist_node_t *tail;
};

static void test(int expected, int actual, const char* testName);

void TestFlow1();
void TestFlow2();
void TestFlow3();
void TestFlow4();
void TestFlow5();
void TestFlow6();
void TestFlow7();

int main()
{
	
	TestFlow1();
	TestFlow2();
	TestFlow3();
	TestFlow4();
	TestFlow5();
	TestFlow6();
	TestFlow7();

	return 0;
}

void TestFlow1()
{
	queue_t *queue = QueCreate();
	QueDestroy(queue);
}

void TestFlow2()
{
	queue_t *queue = QueCreate();

	puts("Flow2");		
	
	test(0, QueSize(queue), "Size");
	test(1, QueIsEmpty(queue), "IsEmpty");
	QueDestroy(queue);
	puts(" "); 
}

void TestFlow3()
{
	const int arr[] = {1, 2, 3, 4, 5, 6};	
	queue_t *queue = QueCreate();

	puts("Flow3");		
	
	QueEnqueue(queue, arr);
	test(1, QueSize(queue), "Size");
	test(0, QueIsEmpty(queue), "IsEmpty");
	test(1, *(int*)QuePeek(queue), "Peek");
	QueDestroy(queue);
	puts(" "); 
}

void TestFlow4()
{
	const int arr[] = {1, 2, 3, 4, 5, 6};	
	queue_t *queue = QueCreate();
	int i = 0;

	puts("Flow4");	
	
	for(; i < 6; ++i)
	{
		QueEnqueue(queue, arr + i);
	}

	test(6, QueSize(queue), "Size");
	test(1, *(int*)QuePeek(queue), "Peek");

	QueDequeue(queue);

	test(5, QueSize(queue), "Size");
	test(2, *(int*)QuePeek(queue), "Peek");
	QueDestroy(queue);
	puts(" ");
}

void TestFlow5()
{
	const int arr[] = {1, 2, 3, 4, 5, 6};	
	queue_t *queue1 = QueCreate();
	queue_t *queue2 = QueCreate();
	int i = 0;

	puts("Flow5");	
	
	for(; i < 3; ++i)
	{
		QueEnqueue(queue1, arr + i);
	}

	for(; i < 6; ++i)
	{
		QueEnqueue(queue2, arr + i);
	}

	queue1 = QueAppend(queue1, queue2);

	test(6, QueSize(queue1), "Size");
	test(1, *(int*)QuePeek(queue1), "Peek");

	QueDequeue(queue1);
	QueDequeue(queue1);
	QueDequeue(queue1);

	test(3, QueSize(queue1), "Size");
	test(4, *(int*)QuePeek(queue1), "Peek");

	QueDestroy(queue1);
	puts(" ");
}

void TestFlow6()
{
	const int arr[] = {1, 2, 3, 4, 5, 6};	
	queue_t *queue1 = QueCreate();
	queue_t *queue2 = QueCreate();
	int i = 0;

	puts("Flow6");	

	for(; i < 6; ++i)
	{
		QueEnqueue(queue1, arr + i);
	}

	queue1 = QueAppend(queue1, queue2);

	test(6, QueSize(queue1), "Size");
	test(1, *(int*)QuePeek(queue1), "Peek");

	QueDequeue(queue1);
	QueDequeue(queue1);
	QueDequeue(queue1);

	test(3, QueSize(queue1), "Size");
	test(4, *(int*)QuePeek(queue1), "Peek");

	QueDestroy(queue1);
	puts(" ");
}

void TestFlow7()
{
	const int arr[] = {1, 2, 3, 4, 5, 6};	
	queue_t *queue1 = QueCreate();
	queue_t *queue2 = QueCreate();
	int i = 0;

	puts("Flow7");	

	for(; i < 6; ++i)
	{
		QueEnqueue(queue2, arr + i);
	}

	queue1 = QueAppend(queue1, queue2);

	test(6, QueSize(queue1), "Size");
	test(1, *(int*)QuePeek(queue1), "Peek");

	QueDequeue(queue1);
	QueDequeue(queue1);
	QueDequeue(queue1);

	test(3, QueSize(queue1), "Size");
	test(4, *(int*)QuePeek(queue1), "Peek");

	QueDestroy(queue1);
	puts(" ");
}
static void test(int expected, int actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %d actual: %d\n", expected, actual);
    }
}
