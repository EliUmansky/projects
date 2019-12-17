#include <stdio.h> /* size_t */
#include <string.h> /* strcmp */
#include "dlist.h"
#include "sortlist.h"
#include "pqueue.h"

typedef struct data
{
	int priority;
	char *name;
} data_t;

static void test(int expected, int actual, const char* testName);
static int IsPriorityBefore(const void *new_data, const void *data, void *param);
static int NameCmp(const void *str1, const void *name_struct);

void TestFlow1();
void TestFlow2();
void TestFlow3();
void TestFlow4();

int main()
{

	TestFlow1();
	TestFlow2();
	TestFlow3();
	TestFlow4();

	return 0;
}

void TestFlow1()
{
	int a = 2;	
	p_queue_t *pqueue = PQCreate(IsPriorityBefore, (void*)&a);

	puts("Flow1: Create and Destroy\n");

	test(0, PQSize(pqueue), "|| Size before insert- 0");
	test(1, PQIsEmpty(pqueue), "|| Is list empty- 1");
	PQDestroy(pqueue);
	puts(" ");
}

void TestFlow2()
{
	data_t arr[] = { {4, "Avraham"}, {2, "Shlomo"}, {3, "Moshe"}, {1, "Israel"} };		
	int i = 0;
	p_queue_t *pqueue = PQCreate(IsPriorityBefore, NULL);
	char str[] = "Shlomo";
	char str2[] = "Israel";

	puts("Flow2: Enqueue, Size, IsEmpty, Peek, Dequeue");

	for (; i < 4; ++i)
	{
		PQEnqueue(pqueue, arr + i);		
	}
	
	test(4, PQSize(pqueue), "|| Size after 4 inserts- 4");
	test(0, PQIsEmpty(pqueue), "|| Is list empty- 0");

	test(0, strcmp(str2, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	
	test(3, PQSize(pqueue), "|| Size after Dequeue- 3");

	test(0, strcmp(str, ((data_t*)PQPeek(pqueue))->name), "|| Peek compare");

	PQClear(pqueue);
	test(1, PQIsEmpty(pqueue), "|| Is list empty after Clear- 1");
	
	PQDestroy(pqueue);
	puts(" ");
}

void TestFlow3()
{
	data_t arr[] = { {4, "Avraham"}, {2, "Shlomo"}, {3, "Moshe"}, {1, "Israel"},
					 {5, "Charlie"} };		
	int i = 0;
	p_queue_t *pqueue = PQCreate(IsPriorityBefore, NULL);
	char str1[] = "Israel";
	char str2[] = "Shlomo";
	char str3[] = "Moshe";
	char str4[] = "Avraham";
	char str5[] = "Charlie";

	puts("Flow3: Erase an existing element");

	for (; i < 5; ++i)
	{
		PQEnqueue(pqueue, arr + i);		
	}
	test(5, PQSize(pqueue), "|| Size after 5 Enqueues- 5");

	PQErase(pqueue, &NameCmp, (void*)str2);

	test(0, strcmp(str1, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(0, strcmp(str3, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(0, strcmp(str4, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(0, strcmp(str5, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(1, PQIsEmpty(pqueue), "|| Is list empty- 1");
	PQDestroy(pqueue);
	puts(" ");
}

void TestFlow4()
{
	data_t arr[] = { {4, "Avraham"}, {2, "Shlomo"}, {3, "Moshe"}, {1, "Israel"},
					 {5, "Charlie"} };		
	int i = 0;
	p_queue_t *pqueue = PQCreate(IsPriorityBefore, NULL);
	char str1[] = "Israel";
	char str2[] = "Shlomo";
	char str3[] = "Moshe";
	char str4[] = "Avraham";
	char str5[] = "Charlie";
	char str6[] = "Nissim";

	puts("Flow4: Erase an non existing element");

	for (; i < 5; ++i)
	{
		PQEnqueue(pqueue, arr + i);		
	}
	test(5, PQSize(pqueue), "|| Size after 5 Enqueues- 5");

	PQErase(pqueue, &NameCmp, (void*)str6);

	test(0, strcmp(str1, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(0, strcmp(str2, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(0, strcmp(str3, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(0, strcmp(str4, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(0, strcmp(str5, ((data_t*)PQDequeue(pqueue))->name), "|| Dequeue compare");
	test(1, PQIsEmpty(pqueue), "|| Is list empty- 1");
	PQDestroy(pqueue);
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

static int IsPriorityBefore(const void *new_data, const void *data, void *param)
{	
	int new_priority = ((data_t*)new_data)->priority;
	int old_priority = ((data_t*)data)->priority;

	(void)param;

	return (new_priority - old_priority);
}
/*
static int FindInt(const void *data, const void *param)
{
	return (*(int*)data == *(int*)param);
}
*/
static int NameCmp(const void *name_struct, const void *str)
{	
	return (0 == strcmp((char*)str, ((data_t*)name_struct)->name));
}
