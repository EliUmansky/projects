#include <stdlib.h> /* size_t */
#include <stdio.h> /* printf */

#include "search.h"

void BSearchTest();
void RecBSearchTest();
void JumpSearchTest();
static void test(int expected, int actual, const char* testName);
static void addresstest(void *expected, void *actual, const char* testName);
static int CompareFunc(const void *data_to_find, const void *data);

int main()
{
	BSearchTest();
	RecBSearchTest();
	JumpSearchTest();

	return 0;
}

void BSearchTest()
{
	int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int arr2[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};	
	int num = 6, num2 = 9, num3 = 10, num4 = 11;

	puts("\nBinary search");
	puts("-------------");
	test(6, *(int*)BSearch(arr, 10, sizeof(int), &CompareFunc, &num),
	"Looking for existing element");
	test(9, *(int*)BSearch(arr, 10, sizeof(int), &CompareFunc, &num2),
	"Looking for existing element");
	test(10, *(int*)BSearch(arr, 10, sizeof(int), &CompareFunc, &num3),
	"Looking for existing element");
	addresstest(NULL, BSearch(arr, 10, sizeof(int), &CompareFunc, &num4),
	"Looking for non-existing element\n");

	test(6, *(int*)BSearch(arr2, 11, sizeof(int), &CompareFunc, &num),
	"Looking for existing element");
	test(9, *(int*)BSearch(arr2, 11, sizeof(int), &CompareFunc, &num2),
	"Looking for existing element");
	test(10, *(int*)BSearch(arr2, 11, sizeof(int), &CompareFunc, &num3),
	"Looking for existing element");
	test(11, *(int*)BSearch(arr2, 11, sizeof(int), &CompareFunc, &num4),
	"Looking for existing element");
}
void RecBSearchTest()
{
	int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int arr2[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};	
	int num = 6, num2 = 9, num3 = 10, num4 = 11;

	puts("\nRec binary search");
	puts("-----------------");
	test(6, *(int*)BSearch(arr, 10, sizeof(int), &CompareFunc, &num),
	"Looking for existing element");
	test(9, *(int*)BSearch(arr, 10, sizeof(int), &CompareFunc, &num2),
	"Looking for existing element");
	test(10, *(int*)BSearch(arr, 10, sizeof(int), &CompareFunc, &num3),
	"Looking for existing element");
	addresstest(NULL, BSearch(arr, 10, sizeof(int), &CompareFunc, &num4),
	"Looking for non-existing element\n");

	test(6, *(int*)BSearch(arr2, 11, sizeof(int), &CompareFunc, &num),
	"Looking for existing element");
	test(9, *(int*)BSearch(arr2, 11, sizeof(int), &CompareFunc, &num2),
	"Looking for existing element");
	test(10, *(int*)BSearch(arr2, 11, sizeof(int), &CompareFunc, &num3),
	"Looking for existing element");
	test(11, *(int*)BSearch(arr2, 11, sizeof(int), &CompareFunc, &num4),
	"Looking for existing element");
}

void JumpSearchTest()
{
	int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int arr2[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};	
	int i = 0, num = 11, num2 = 0;

	puts("\nJump search");
	puts("-----------");

	for (i = 0; i < 10; ++i)
	{
		test(i + 1, *(int*)JumpSearch(arr, 10, sizeof(int), &CompareFunc, arr + i),
		"Looking for existing element");
	}
	addresstest(NULL, JumpSearch(arr, 10, sizeof(int), &CompareFunc, &num),
	"Looking for non-existing element");
	addresstest(NULL, JumpSearch(arr, 10, sizeof(int), &CompareFunc, &num2),
	"Looking for non-existing element\n");

	for (i = 0; i < 11; ++i)
	{
		test(i + 1, *(int*)JumpSearch(arr2, 11, sizeof(int), &CompareFunc, arr2 + i),
		"Looking for existing element");
	}
	addresstest(NULL, JumpSearch(arr2, 11, sizeof(int), &CompareFunc, &num2),
	"Looking for non-existing element\n");
}

static int CompareFunc(const void *data_to_find, const void *data)
{
	return (*(int*)data_to_find - *(int *)data);
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
