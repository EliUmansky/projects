#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst.h"

#define SIZE (11)

void CreateDestroy();
void InsertSizeBeginEnd();
void ForEach();
void Remove();
void PrevFind();

static void test(int expected, int actual, const char* testName);
static int CmpFunc(const void *new_data, const void *data, void *param);
static int DataPlusParam(void *data, void *param);

int main()
{
	CreateDestroy();
	InsertSizeBeginEnd();
	ForEach();
	Remove();
	PrevFind();
    
	return 0;
}

void CreateDestroy()
{
	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	bst_t *bst = BSTCreate(CmpFunc, NULL);
	int i = 0;

	puts("Test 1 - Create and Destroy\n");

	BSTInsert(bst, arr + i);

	BSTDestroy(bst);
}
	
void InsertSizeBeginEnd()
{
	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	bst_t *bst = BSTCreate(CmpFunc, NULL);
	int i = 0;
	bst_iter_t iter = NULL;

	puts("Test 2 - Insert, Next, Size, IsEmpty\n");

	test(1, BSTIsEmpty(bst), "IsEmpty - Yep");
	test(0, BSTSize(bst), "Size = 0");

	for (; i < SIZE; ++i)
	{
		iter = BSTInsert(bst, arr + i);
		test(i + 1, BSTSize(bst), "Size after i inserts");
		test(*(arr + i), *(int*)BSTGetData(iter), "Element value\n");
	}

	test(0, BSTIsEmpty(bst), "IsEmpty - Nope");
	test(11, BSTSize(bst), "Size = 11");

	BSTDestroy(bst);
}

void ForEach()
{
	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	int sorted_arr[SIZE] = {2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14};
	bst_t *bst = BSTCreate(CmpFunc, NULL);
	int i = 0, param = 1, data = 0;
	bst_iter_t iter1 = NULL, iter2 = NULL;

	puts("Test 3 - Insert Order, ForEach\n");

	for (; i < SIZE; ++i)
	{
		BSTInsert(bst, arr + i);
	}
	
	iter1 = BSTBegin(bst);
	iter2 = BSTEnd(bst);

	for (i = 0; i < SIZE; ++i)
	{		
		data = *(int*)BSTGetData(iter1);	
		test(sorted_arr[i], data, "Is Insertion Correct");
		iter1 = BSTNext(iter1);
	}

	iter1 = BSTBegin(bst);
	i = BSTForEach(iter1, iter2, DataPlusParam, &param);

	test(0, i, "ForEach succeeded\n");

	for (i = 0; i < SIZE; ++i)
	{		
		data = *(int*)BSTGetData(iter1);	
		test(sorted_arr[i] + 1, data, "Is Foreach Correct");
		iter1 = BSTNext(iter1);
	}

	BSTDestroy(bst);	
}

void Remove()
{
	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	int sorted_arr[SIZE] = {2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14};
	int after_remove[SIZE - 3] = {2, 4, 5, 8, 9, 11, 12, 14};
	bst_t *bst = BSTCreate(CmpFunc, NULL);
	int i = 0, param = 1, data = 0;
	bst_iter_t iter1 = NULL;
	bst_iter_t rem_no_child = NULL, rem_one_left_child = NULL, rem_both = NULL;

	puts("Test 4 - Remove\n");

	rem_both = BSTInsert(bst, arr + 0);
	BSTInsert(bst, arr + 1);
	BSTInsert(bst, arr + 2);
	BSTInsert(bst, arr + 3);
	BSTInsert(bst, arr + 4);
	rem_one_left_child = BSTInsert(bst, arr + 5);
	rem_no_child = BSTInsert(bst, arr + 6);
	BSTInsert(bst, arr + 7);
	BSTInsert(bst, arr + 8);
	BSTInsert(bst, arr + 9);
	BSTInsert(bst, arr + 10);

	BSTRemove(rem_no_child);
	BSTRemove(rem_one_left_child);
	BSTRemove(rem_both);

	iter1 = BSTBegin(bst);

	for (i = 0; i < (SIZE - 3); ++i)
	{		
		data = *(int*)BSTGetData(iter1);	
		test(after_remove[i], data, "Is Remove Correct");
		iter1 = BSTNext(iter1);
	}

	BSTDestroy(bst);	
}

void PrevFind()
{
	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	int sorted_arr[SIZE] = {2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14};
	bst_t *bst = BSTCreate(CmpFunc, NULL);
	int i = 0, param = 1, data = 0;
	bst_iter_t iter1 = NULL;
	
	puts("Test 4 - Prev, Find\n");

	iter1 = BSTBegin(bst);

	for (; i < SIZE; ++i)
	{
		iter1 = BSTInsert(bst, arr + i);
	}

	for(i = 8; i >= 0; --i)
	{
		test(sorted_arr[i], *(int*)BSTGetData(iter1), "Get Data");
		iter1 = BSTPrev(iter1);
	}

	BSTDestroy(bst);	
}
static int CmpFunc(const void *new_data, const void *data, void *param)
{
    (void)param;

	return (*(int*)new_data - *(int*)data);
}

static int DataPlusParam(void *data, void *param)
{
	*(int*)data = *(int*)data + *(int*)param;

	return 0;
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
