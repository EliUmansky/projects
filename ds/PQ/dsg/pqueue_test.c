/****************************
     Priority Queue
        Test File
    Author : Greg Markovsky
      Date : 10/12/2019 
****************************/

#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "tests.h" /* TestStart */
#include "pqueue.h" /* PQCreate */

static void TestNewPriorityQueue(int voc);
static void TestEnqueuePeekDequeue(int voc);
static void TestClear(int voc);
static void TestErase(int voc);

static int ComparePeople(const void *new_data, const void *data, void *param);
static char *GetPersonName(void *data);
static int MatchPersonName(const void *data, const void *param);

typedef struct 
{
	int priority;
	char *name;
} person_t;

int main()
{
	/* pick SILENT or VERBOSE for additional data */

	RUN_TEST( VERBOSE , TestNewPriorityQueue);
	RUN_TEST( VERBOSE , TestEnqueuePeekDequeue);
	RUN_TEST( VERBOSE , TestClear);
	RUN_TEST( VERBOSE , TestErase);

	return 0;
}

/* --------------------------------------------- Test New Priority Queue --- */

static void TestNewPriorityQueue(int voc)
{
	p_queue_t *pqueue = NULL;
	char *test_conditions = "Create: new. " 
							"IsEmpty: empty. "
							"Size: 0. "
							"Destroy: empty. "; 

	TestStart("New Priority Queue", test_conditions);
	pqueue = PQCreate(&ComparePeople, NULL);
	TestNotNull(voc, "Create: new.", pqueue);
	TestInt(voc, "IsEmpty: empty.", 1, PQIsEmpty(pqueue));
	TestSizeT(voc, "Size: 0.", 0, PQSize(pqueue));
	PQDestroy(pqueue); 
}

/* ------------------------------------------- Test Enqueue Peek Dequeue --- */

static void TestEnqueuePeekDequeue(int voc)
{
	p_queue_t *pqueue = NULL;
	person_t list[] = { {4, "Dany"}, {7, "Amir"}, {27, "ingrid"},
					    {0, "Yosi"}, {2, "Jake"}, {-3, "Samuel"},
						{4, "Mark"}, {4, "Kate"}, {-100, "Paul"},
						{1, "John"}, {10, "Jim"}, {70, "Tamara"} };
	void *data = NULL;
	char *line[] = { "Paul", "Samuel", "Yosi", "John", "Jake", "Dany", "Mark", 
					 "Kate", "Amir", "Jim", "ingrid", "Tamara" };
	size_t i = 0;
	size_t size = sizeof(list) / sizeof(*list);
	char *test_conditions = "IsEmpty: not empty. "
							"Size: 1-11. "
							"Destroy: not empty. "
							"Enqueue: empty, not empty, in order, not in order"
							"Peek: basic"
							"Dequeue: full queue, single"; 

	TestStart("Enqueue & Dequeue", test_conditions);
	pqueue = PQCreate(&ComparePeople, NULL);
	for (i = 0; i < size; ++i)
	{
		TestSizeT(voc, "Size: not empty", i, PQSize(pqueue));
		PQEnqueue(pqueue, (list + i));
	}
	TestStr(voc, "Peek: basic", line[0], GetPersonName(PQPeek(pqueue)));	
	TestInt(voc, "IsEmpty: not empty.", 0, PQIsEmpty(pqueue));
	for (i = 0; i < size; ++i)
	{
		data = PQDequeue(pqueue);
		TestStr(voc, "Enq/Deq", line[i], GetPersonName(data));
	}
	PQDestroy(pqueue); 
}

/* ---------------------------------------------------------- Test Clear --- */

static void TestClear(int voc)
{
	p_queue_t *pqueue = NULL;
	person_t list[] = { {4, "Dany"}, {7, "Amir"}, {27, "ingrid"},
					    {0, "Yosi"}, {2, "Jake"}, {-3, "Samuel"},
						{4, "Mark"}, {4, "Kate"}, {-100, "Paul"},
						{1, "John"}, {10, "Jim"}, {70, "Tamara"} };
	size_t i = 0;
	size_t size = sizeof(list) / sizeof(*list);
	char *test_conditions = "Clear: clear all"; 

	TestStart("Clear", test_conditions);
	pqueue = PQCreate(&ComparePeople, NULL);
	for (i = 0; i < size; ++i)
	{
		PQEnqueue(pqueue, (list + i));
	}
	TestSizeT(voc, "Clear: size before", 12, PQSize(pqueue));
	PQClear(pqueue);
	TestSizeT(voc, "Clear: size before", 0, PQSize(pqueue));
	TestInt(voc, "Clear: check empty.", 1, PQIsEmpty(pqueue));
	PQDestroy(pqueue); 
}

/* ---------------------------------------------------------- Test Erase --- */

static void TestErase(int voc)
{
	p_queue_t *pqueue = NULL;
	person_t list[] = { {4, "Dany"}, {7, "Amir"}, {27, "ingrid"},
					    {0, "Yosi"}, {2, "Jake"}, {-3, "Samuel"},
						{4, "Mark"}, {4, "Kate"}, {-100, "Paul"},
						{1, "John"}, {10, "Jim"}, {70, "Tamara"} };
	char *line[] = { "Paul", "Samuel", "Yosi", "John", "Dany", "Mark", 
					 "Kate", "Amir", "Jim", "ingrid", "Tamara" };

	char *kill_jake = "Jake";
	char *kill_fake = "fake";
	size_t i = 0;
	void *data;
	size_t size = sizeof(list) / sizeof(*list);
	char *test_conditions = "Erase: mid"; 

	TestStart("Erase", test_conditions);
	pqueue = PQCreate(&ComparePeople, NULL);
	for (i = 0; i < size; ++i)
	{
		PQEnqueue(pqueue, (list + i));
	}
	TestSizeT(voc, "Erase: size before kill", size, PQSize(pqueue));
	
	PQErase(pqueue, &MatchPersonName, kill_jake);	
	TestSizeT(voc, "Erase: size after kill", --size, PQSize(pqueue));	

	PQErase(pqueue, &MatchPersonName, kill_fake);
	TestSizeT(voc, "Erase: size after fake kill", size, PQSize(pqueue));
	TestInt(voc, "Erase: keeps end.", 0, PQIsEmpty(pqueue));

	for (i = 0; i < size; ++i)
	{
		data = PQDequeue(pqueue);
		TestStr(voc, "Erase: queue after kill", line[i], GetPersonName(data));
	}
	
	PQDestroy(pqueue); 
}

/* ---------------------------- Service Functions -------------------------- */
/* ---------------------------------------------------- Compare Integers --- */

static int ComparePeople(const void *new_data, const void *data, void *param)
{
	int new_priority = ((person_t *)new_data)->priority;
	int priority = ((person_t *)data)->priority;

	(void)param;

	return (new_priority - priority);
}

static char *GetPersonName(void *data)
{
	return ((person_t *)data)->name;
}

static int MatchPersonName(const void *data, const void *param)
{
	if (0 == strcmp(GetPersonName((void *)data), (char *)param))
	{
		return 1;
	}
	
	return 0;
}








