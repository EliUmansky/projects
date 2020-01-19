#include <stdio.h> /* size_t */
#include "dlist.h"
#include "sortlist.h"

#define INTERNAL_ITER (sorted_iter.internal_iter)

static void test(int expected, int actual, const char* testName);
/*static void addresstest(void *expected, void *actual, const char* testName);*/
static int IsIntBefore(const void *new_data, const void *data, void *param);
static int DataPlusParam(void *data, void *param);
static int FindInt(const void *data, const void *param);

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
	int a = 2;	
	sorted_list_t *sorted_list = SortedListCreate(IsIntBefore, (void*)&a);

	puts("Flow1: Create and Destroy\n");

	test(0, SortedListSize(sorted_list), "|| Size before insert- 0");
	test(1, SortedListIsEmpty(sorted_list), "|| Is list empty- 1");
	SortedListDestroy(sorted_list);
	puts(" ");
}

void TestFlow2()
{
	int arr[] = {4, 2, 1, 3, 5};		
	int i = 0;	
	int hold_data = 0;
	sorted_list_iter_t sorted_iter = {0};

	sorted_list_t *sorted_list = SortedListCreate(IsIntBefore, (void*)&arr);

	puts("Flow2: Insert, PopFront/Back, GetData, IsSameIter, Remove, Find\n");

	for (; i < 5; ++i)
	{
		SortedListInsert(sorted_list, (void*)(arr + i));
	}
	
	test(5, SortedListSize(sorted_list), "|| Size after 5 inserts- 5");
	test(0, SortedListIsEmpty(sorted_list), "|| Is list empty- 0");

	hold_data = *(int*)SortedListPopFront(sorted_list);
	test(1, hold_data, "|| PopFront- 1");
	hold_data = *(int*)SortedListPopFront(sorted_list);
	test(2, hold_data, "|| PopFront- 2");
	hold_data = *(int*)SortedListPopBack(sorted_list);
	test(5, hold_data, "|| PopBack- 5");
	hold_data = *(int*)SortedListPopBack(sorted_list);
	test(4, hold_data, "|| PopBack- 4");

	hold_data = *(int*)SortedListGetData(SortedListBegin(sorted_list));
	test(3, hold_data, "|| GetData- 3");	

	sorted_iter = SortedListPrev(SortedListEnd(sorted_list));
	test(1, SortedListIsSameIter(sorted_iter, SortedListBegin(sorted_list)), 
		 "|| IsSameIterator- 1");

	sorted_iter = SortedListNext(sorted_iter);
	test(0, SortedListIsSameIter(sorted_iter, SortedListBegin(sorted_list)), 
		 "|| IsSameIterator- 0");

	SortedListRemove(SortedListPrev(sorted_iter));
	
	test(0, SortedListSize(sorted_list), "|| Size before insert- 0");
	test(1, SortedListIsEmpty(sorted_list), "|| Is list empty- 1");

	SortedListDestroy(sorted_list);
	puts(" ");
}

void TestFlow3()
{
	int arr[] = {4, 2, 1, 3, 5, 8, 6 ,7};		
	int i = 0, a = 1;	
	int hold_data = 0;
	sorted_list_iter_t sorted_iter = {0};

	sorted_list_t *sorted_list = SortedListCreate(IsIntBefore, (void*)&arr);

	puts("Flow3: ForEach, FindIf\n");

	for (; i < 8; ++i)
	{
		SortedListInsert(sorted_list, (void*)(arr + i));
	}

	SortedListForEach(SortedListBegin(sorted_list),
								 SortedListEnd(sorted_list), DataPlusParam, &a);

	a = 4;

	sorted_iter = SortedListFindIf(SortedListBegin(sorted_list),
					   SortedListPrev(SortedListEnd(sorted_list)), FindInt, &a);

	hold_data = *(int*)SortedListGetData(sorted_iter);

	test(4, hold_data, "|| ForEach + FindIf");

	hold_data = *(int*)SortedListPopFront(sorted_list);
	test(2, hold_data, "|| PopFront- 2");
	hold_data = *(int*)SortedListPopFront(sorted_list);
	test(3, hold_data, "|| PopFront- 3");
	hold_data = *(int*)SortedListPopBack(sorted_list);
	test(9, hold_data, "|| PopBack- 9");
	hold_data = *(int*)SortedListPopBack(sorted_list);
	test(8, hold_data, "|| PopBack- 8");

	SortedListDestroy(sorted_list);
	puts(" ");
}

void TestFlow4()
{
	int arr[] = {6, 5, 8, 7, 4, 3, 1, 2};		
	int i = 0;	
	
	sorted_list_t *sorted_list_1 = SortedListCreate(IsIntBefore, (void*)&arr);
	sorted_list_t *sorted_list_2 = SortedListCreate(IsIntBefore, (void*)&arr);

	puts("Flow4: Merge larger to smaller\n");

	for (; i < 4; ++i)
	{
		SortedListInsert(sorted_list_1, (void*)(arr + i));
	}

	for (; i < 8; ++i)
	{
		SortedListInsert(sorted_list_2, (void*)(arr + i));
	}

	SortedListMerge(sorted_list_1, sorted_list_2);
	test(8, SortedListSize(sorted_list_1), "|| Size after merge- 8");

	SortedListDestroy(sorted_list_1);
	SortedListDestroy(sorted_list_2);
	puts(" ");
}

void TestFlow5()
{
	int arr[] = {5, 6, 2, 1, 3, 4, 8 ,7};		
	int i = 0;	

	sorted_list_t *sorted_list_1 = SortedListCreate(IsIntBefore, (void*)&arr);
	sorted_list_t *sorted_list_2 = SortedListCreate(IsIntBefore, (void*)&arr);

	puts("Flow4: Merge smaller to larger\n");

	for (; i < 4; ++i)
	{
		SortedListInsert(sorted_list_1, (void*)(arr + i));
	}

	for (; i < 8; ++i)
	{
		SortedListInsert(sorted_list_2, (void*)(arr + i));
	}

	SortedListMerge(sorted_list_1, sorted_list_2);
	test(8, SortedListSize(sorted_list_1), "|| Size after merge- 8");

	SortedListDestroy(sorted_list_1);
	SortedListDestroy(sorted_list_2);
	puts(" ");
}

void TestFlow6()
{
	int arr[] = {5, 6, 2, 1};		
	int i = 0;	

	sorted_list_t *sorted_list_1 = SortedListCreate(IsIntBefore, (void*)&arr);
	sorted_list_t *sorted_list_2 = SortedListCreate(IsIntBefore, (void*)&arr);

	puts("Flow4: Merge 0 to n & n to 0\n");

	for (; i < 4; ++i)
	{
		SortedListInsert(sorted_list_1, (void*)(arr + i));
	}

	SortedListMerge(sorted_list_1, sorted_list_2);
	test(4, SortedListSize(sorted_list_1), "|| Size after merge- 4");

	SortedListDestroy(sorted_list_1);
	SortedListDestroy(sorted_list_2);

	sorted_list_1 = SortedListCreate(IsIntBefore, (void*)&arr);
	sorted_list_2 = SortedListCreate(IsIntBefore, (void*)&arr);

	for (i = 0; i < 4; ++i)
	{
		SortedListInsert(sorted_list_2, (void*)(arr + i));
	}

	SortedListMerge(sorted_list_1, sorted_list_2);
	test(4, SortedListSize(sorted_list_1), "|| Size after merge- 4");

	SortedListDestroy(sorted_list_1);
	SortedListDestroy(sorted_list_2);	
	puts(" ");
}

void TestFlow7()
{
	int arr[] = {12, 65, 4, 32, 15, 6, 35, 54, 84};		
	int i = 0;	
	int hold_data = 0;
	sorted_list_iter_t sorted_iter = {0};
	sorted_list_t *sorted_list = SortedListCreate(IsIntBefore, (void*)&arr);

	puts("Flow7: Find\n");

	for (; i < 9; ++i)
	{
		SortedListInsert(sorted_list, (void*)(arr + i));
	}
	test(9, SortedListSize(sorted_list), "|| Size- 9");
	
	i = 32;
	sorted_iter = SortedListFind(sorted_list, SortedListBegin(sorted_list),
								 SortedListEnd(sorted_list), &i);
	hold_data = *(int*)SortedListGetData(sorted_iter);	
	test(i, hold_data, "|| Find existing element");	
	
	i = 4;
	sorted_iter = SortedListFind(sorted_list, SortedListBegin(sorted_list),
								 SortedListEnd(sorted_list), &i);	
	hold_data = *(int*)SortedListGetData(sorted_iter);		
	test(i, hold_data, "|| Find first element");	

	i = 84;
	sorted_iter = SortedListFind(sorted_list, SortedListBegin(sorted_list),
								 SortedListEnd(sorted_list), &i);	
	hold_data = *(int*)SortedListGetData(sorted_iter);		
	test(i, hold_data, "|| Find last element");	
/*
	i = 5;
	sorted_iter = SortedListFind(sorted_list, SortedListBegin(sorted_list),
								 SortedListEnd(sorted_list), &i);	
	hold_data = *(int*)SortedListGetData(sorted_iter);		
	test(SortedListBegin(sorted_list, sorted_iter, "|| Find non-existing element");	

*/
	SortedListDestroy(sorted_list);	
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
/*
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
*/
static int IsIntBefore(const void *new_data, const void *data, void *param)
{
	(void)param;	
	
	return (*(int*)new_data < *(int*)data);
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


