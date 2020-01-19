#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"

#define SIZE (11)
#define NEW_LINE (printf("\n"))

void CreateDestroy();
void InsertSizeIsEmpty();
void Height();
void Remove();

static void test(int expected, int actual, const char* testName);
static int CmpFunc(const void *new_data, const void *data, void *param);
static int PrintData(void *data, void *param);

int main()
{
	CreateDestroy();
	InsertSizeIsEmpty();
	Height();
	Remove();
	   
	return 0;
}

void CreateDestroy()
{
	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	avl_t *avl = AVLCreate(CmpFunc, NULL);
	int i = 0;

	puts("\nTest 1 - Create and Destroy");
	puts("-----------------------------");

	AVLInsert(avl, arr + i);

	AVLDestroy(avl);
}

void InsertSizeIsEmpty()
{
	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	int sorted_arr[SIZE] = {2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14};
	avl_t *avl = AVLCreate(CmpFunc, NULL);
	int i = 0;

	puts("\nTest 2 - Insert, Size, IsEmpty, ForEach");
	puts("-----------------------------------------");

	test(1, AVLIsEmpty(avl), "IsEmpty - Yep");
	test(0, AVLSize(avl), "Size = 0");

	for (; i < SIZE; ++i)
	{
		AVLInsert(avl, arr + i);		
		test(i + 1, AVLSize(avl), "Size after i inserts");
	}

	test(0, AVLIsEmpty(avl), "IsEmpty - Nope");
	test(11, AVLSize(avl), "Size = 11");
	puts("\nExpected order of elements: ");
	i = AVLForEach(avl, PrintData, NULL);
	NEW_LINE;
	puts("Actual order of elements:   ");
	for (; i < SIZE; ++i)
	{
		printf("%d ", sorted_arr[i]);
	}

	AVLDestroy(avl);
}

void Height()
{
	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	avl_t *avl = AVLCreate(CmpFunc, NULL);

	puts("\n\nTest 3 - Height");
	puts("-----------------");

	test(0, AVLHeight(avl), "Height after 0 inserts");	
	AVLInsert(avl, arr + 0);		
	test(1, AVLHeight(avl), "Height after 1 inserts");
	AVLInsert(avl, arr + 1);		
	test(2, AVLHeight(avl), "Height after 2 inserts");
	AVLInsert(avl, arr + 2);		
	test(2, AVLHeight(avl), "Height after 3 inserts");
	AVLInsert(avl, arr + 3);		
	test(3, AVLHeight(avl), "Height after 4 inserts");
	AVLInsert(avl, arr + 4);		
	test(3, AVLHeight(avl), "Height after 5 inserts");
	AVLInsert(avl, arr + 5);		
	test(3, AVLHeight(avl), "Height after 6 inserts");
	AVLInsert(avl, arr + 6);		
	test(4, AVLHeight(avl), "Height after 7 inserts");
	AVLInsert(avl, arr + 7);		
	test(4, AVLHeight(avl), "Height after 8 inserts");
	AVLInsert(avl, arr + 8);		
	test(4, AVLHeight(avl), "Height after 9 inserts");
	AVLInsert(avl, arr + 9);		
	test(4, AVLHeight(avl), "Height after 10 inserts");
	AVLInsert(avl, arr + 10);		
	test(4, AVLHeight(avl), "Height after 11 inserts");

	AVLDestroy(avl);
}

void Remove()
{

	int arr[SIZE] = {10, 2, 12, 8, 4, 6, 3, 14, 9, 5, 11};
	int after_remove[SIZE - 3] = {3, 5, 6, 9, 10, 11, 12}; 
	avl_t *avl = AVLCreate(CmpFunc, NULL);
	int i = 0;

	puts("\nTest 4 - Remove");
	puts("-----------------\n");

	for (; i < SIZE; ++i)
	{
		AVLInsert(avl, arr + i);		
	}
	test(11, AVLSize(avl), "Size = 11");

	AVLRemove(avl, arr + 7);
	AVLRemove(avl, arr + 1);
	AVLRemove(avl, arr + 4);
	AVLRemove(avl, arr + 3);

	puts("Expected order of elements: ");
	for (i = 0; i < SIZE - 4; ++i)
	{
		printf("%d ", after_remove[i]);
	}
	NEW_LINE;
	puts("Actual order of elements:   ");
	i = AVLForEach(avl, PrintData, NULL);
	NEW_LINE;
	test(7, AVLSize(avl), "Size = 7");
	test(4, AVLHeight(avl), "Height after 3 removes");

	AVLDestroy(avl);	
}

static int CmpFunc(const void *new_data, const void *data, void *param)
{
    (void)param;

	return (*(int*)new_data - *(int*)data);
}

static int PrintData(void *data, void *param)
{
	(void)param;

	printf("%d ", *(int*)data);

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
