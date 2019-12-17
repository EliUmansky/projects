#include <stdio.h> /* size_t */
#include "dlist.h"

static void test(int expected, int actual, const char* testName);
static void addresstest(void *expected, void *actual, const char* testName);
static int DataPlusParam(void *data, void *param);
static int FindInt(const void *data, const void *param);

void TestFlow1();
void TestFlow2();
void TestFlow3();
void TestFlow4();
void TestFlow5();

int main()
{

	TestFlow1();
	TestFlow2();
	TestFlow3();
	TestFlow4();
	TestFlow5();

	return 0;
}

void TestFlow1()
{
	dlist_t *dlist = DListCreate();

	puts("Flow1");

	test(0, DListSize(dlist), "|| Size before push- 0");
	test(1, DListIsEmpty(dlist), "|| Is list empty- 1");
	DListDestroy(dlist);
	puts(" ");
}

void TestFlow2()
{
	dlist_t *dlist = DListCreate();
	int a = 2, b = 3;
	int result = 0;

	puts("Flow2");

	DListPushFront(dlist, (void*)&a);
	test(0, DListIsEmpty(dlist), "|| Is list empty after push- 0");
	test(1, DListSize(dlist), "|| Size after PushFront- 1");

	DListPushBack(dlist, (void*)&b);
	test(2, DListSize(dlist), "|| Size after PushBack- 2");

	result = *(int*)DListPopFront(dlist);
	test(2, result, "|| PopFront - 2");

	result = *(int*)DListPopBack(dlist);
	test(3, result, "|| PopFront - 3");

	test(1, DListIsEmpty(dlist), "|| Is list empty after pops- 1");

	DListDestroy(dlist);
	puts(" ");
}

void TestFlow3()
{
	dlist_t *dlist = DListCreate();
	dlist_iter_t test_node = NULL;
	int arr[] = {1 ,2, 3, 4, 5, 6, 7, 8, 9};
	int result = 0;

	puts("Flow3");

	DListPushFront(dlist, (void*)arr);
	DListPushFront(dlist, (void*)(arr + 1));

	DListPushBack(dlist, (void*)(arr + 2));
	test_node = DListPushBack(dlist, (void*)(arr + 3));

	DListInsertBefore(dlist, test_node, (void*)(arr + 4));
	test_node = DListInsertBefore(dlist, test_node, (void*)(arr + 5));
	DListInsertBefore(dlist, test_node, (void*)(arr + 6));

	result = *(int*)DListPopFront(dlist);
	test(2, result, "|| PopFront - 2");
	result = *(int*)DListPopFront(dlist);
	test(1, result, "|| PopFront - 1");
	result = *(int*)DListPopFront(dlist);
	test(3, result, "|| PopFront - 3");
	result = *(int*)DListPopFront(dlist);
	test(5, result, "|| PopFront - 5");
	result = *(int*)DListPopFront(dlist);
	test(7, result, "|| PopFront - 7");
	result = *(int*)DListPopFront(dlist);
	test(6, result, "|| PopFront - 6");
	result = *(int*)DListPopFront(dlist);
	test(4, result, "|| PopFront - 4");

	DListDestroy(dlist);
	puts(" ");
}

void TestFlow4()
{
	dlist_t *dlist = DListCreate();
	dlist_iter_t test_node = NULL;
	dlist_iter_t test_node2 = NULL;
	int arr[] = {1 ,2, 3, 4, 5, 6, 7, 8, 9};
	int result = 0, i = 0;
	const int a = 30;
	const int b = 12;
	puts("Flow4");

	for (; i < 8; ++i)
	{
		DListPushBack(dlist, (void*)(arr + i));
	}

	DListForEach(DListBegin(dlist), DListEnd(dlist), DataPlusParam, &i);

	test_node = DListFind(DListBegin(dlist), DListEnd(dlist), FindInt, &a);
	addresstest(DListEnd(dlist), test_node,
				 "|| After not finding required data, iter == dummy");

	addresstest((void*)0xbadcabab, DListGetData(test_node), "|| Address compare");

	test_node = DListFind(DListBegin(dlist), DListEnd(dlist), FindInt, &b);
	test(b, *(int*)DListGetData(test_node), "|| After finding required data");

	test_node = DListNext(test_node);
	test_node2 = DListRemove(test_node);
	test(b + 2, *(int*)DListGetData(test_node2), "|| After removing");

	puts(" ");
}

void TestFlow5()
{
	dlist_t *dlist1 = DListCreate();
	dlist_t *dlist2 = DListCreate();
	dlist_iter_t test_node = NULL;
	dlist_iter_t test_node2 = NULL;
	int arr[] = {1 ,2, 3, 4, 5, 6, 7, 8, 9};
	int result = 0, i = 0;
	const int a = 30;
	const int b = 12;
	puts("Flow5");

	for (; i < 4; ++i)
	{
		DListPushBack(dlist1, (void*)(arr + i));
	}

    for (; i < 8; ++i)
	{
		DListPushBack(dlist2, (void*)(arr + i));
	}

    test(4, DListSize(dlist1), "|| Size after PushFront- dlist1");
	test(4, DListSize(dlist2), "|| Size after PushFront- dlist2");

	DListSplice(DListNext(DListNext(DListBegin(dlist1))), DListBegin(dlist2),
						 DListPrev(DListEnd(dlist2)));

    test(8, DListSize(dlist1), "|| Size after Splice");

    DListPushBack(dlist2, (void*)(arr + i));
    DListSplice(DListNext(DListNext(DListBegin(dlist1))), DListBegin(dlist2),
						DListBegin(dlist2));

    test(9, DListSize(dlist1), "|| Size after Splice");

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
static void addresstest(void *expected, void *actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %p actual: %p\n", expected, actual);
    }
}
static int DataPlusParam(void *data, void *param)
{
	*(int*)data += *(int*)param;

	return 0;
}

static int FindInt(const void *data, const void *param)
{
	return (*(int*)data == *(int*)param);
}


