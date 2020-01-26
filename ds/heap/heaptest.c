#include <stdio.h>
#include <string.h>
#include <stddef.h> /* size_t */
#include <unistd.h>
#include <stdlib.h>

#include "heap.h"
#include "vector.h"

struct heap
{
	d_vector_t *dvec;
	cmp_func_t cmp_func;
};

struct d_vector
{
	size_t element_size;
	void *start;
	size_t size;
	size_t capacity;
};

static void test(int expected, int actual, const char* testName);
static int IntCompare(const void *new_data, const void *data, void *param);
static int MatchInt(const void *data, const void *data_to_find);
void CreateDestroy();
void SizeIsEmptyPeek();
void Pop();
void Remove();

int main()
{
    CreateDestroy();
    SizeIsEmptyPeek();
	Pop();
	Remove();

	return 0;
}

void CreateDestroy()
{
	int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	int i = 0;	
	heap_t *heap = HeapCreate(&IntCompare, NULL);
	
	puts("Create and destroy");
    puts("------------------\n");
    for (i = 0; i < 16; ++i)
    {
        HeapPush(heap, arr + i);
    }

    HeapDestroy(heap);
}

void SizeIsEmptyPeek()
{
	int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    size_t i = 0;
	heap_t *heap = HeapCreate(&IntCompare, NULL);

    puts("\nSize, IsEmpty, Peek");
    puts("------------------\n");

    test(0, HeapSize(heap), "Size before insert = 0");
    test(1, HeapIsEmpty(heap), "IsEmpty before insert = 1");

    for (i = 0; i < 16; ++i)
    {
        HeapPush(heap, arr + i);
    }

    test(16, HeapSize(heap), "Size after inserts = 16");
    test(0, HeapIsEmpty(heap), "IsEmpty after insert = 0");
	test(16, *(int*)HeapPeek(heap), "Peek");

    HeapDestroy(heap);
}

void Pop()
{
	int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int i = 0;
	heap_t *heap = HeapCreate(&IntCompare, NULL);

    puts("\nPop");
    puts("-----\n");

    for (i = 0; i < 16; ++i)
    {
        HeapPush(heap, arr + i);
    }

	for (i = 15; i >= 0; --i)
	{
		test(arr[i], *(int*)HeapPeek(heap), "Peek");
		HeapPop(heap);
	}

    HeapDestroy(heap);
}

void Remove()
{
	int arr[15] = {100, 80, 70, 55, 45, 60, 50, 30, 20, 40, 18, 8, 5, 48, 35};
	int arr2[14] = {100, 55, 70, 35, 45, 60, 50, 30, 20, 40, 18, 8, 5, 48};
	int arr3[15] = {100, 80, 70, 55, 45, 60, 50, 30, 20, 40, 18, 8, 5, 48, 35};
	int arr4[15] = {100, 80, 70, 55, 45, 60, 50, 30, 20, 40, 18, 8, 5, 48, 35};
    int i = 0;
	heap_t *heap = HeapCreate(&IntCompare, NULL);

    puts("\nRemove");
    puts("-----\n");

    for (i = 0; i < 15; ++i)
    {
        HeapPush(heap, arr + i);
    }

	HeapRemove(heap, &MatchInt, arr + 1);

	for (i = 0; i < 14; ++i)
	{
		test(arr2[i], *(int*)DvecGetItemAddress(heap->dvec, i), "After remove");

	}

	i = HeapRemove(heap, &MatchInt, arr + 4);
    HeapDestroy(heap);
}

static int IntCompare(const void *new_data, const void *data, void *param)
{
	(void)param;

	return *(int*)new_data - *(int*)data;
}

static int MatchInt(const void *data, const void *data_to_find)
{
	return (*(int*)data == *(int*)data_to_find);
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
