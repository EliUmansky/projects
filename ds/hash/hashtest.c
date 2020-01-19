#include <stdio.h>
#include <string.h>
#include <stddef.h> /* size_t */
#include <unistd.h>
#include <stdlib.h>

#include "hash.h"
#include "dlist.h"

#define TABLE_SIZE (1000)

static void test(int expected, int actual, const char* testName);
static void addresstest(void *expected, void *actual, const char* testName);
static int DataPlusParam(void *data, void *param);
static int MatchInt(const void *data, const void *param);
static int MatchWord(const void *data, const void *param);
size_t HashFunc(const void *data);
size_t HashFuncInt(const void *data);

void CreateDestroy();
void SizeIsEmpty();
void SimpleDataFindRemove();
void ComplexDataFindRemove();
void ForEach();
void MultipleElements();
void Dictionary();

typedef struct student
{
    char *name;
    size_t birthday;
} student_t;

int main()
{
    CreateDestroy();
    SizeIsEmpty();
    SimpleDataFindRemove();
    ComplexDataFindRemove();
/*    ForEach();
 */   MultipleElements();
    Dictionary();

	return 0;
}

void CreateDestroy()
{
    int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    size_t i = 0, tab_size = 4;
    hash_table_t *hash = HashCreate(tab_size, &MatchInt, &HashFunc);

    puts("Create and destroy");
    puts("------------------\n");
    for (i = 0; i < 16; ++i)
    {
        HashInsert(hash, arr + i);
    }

    HashDestroy(hash);
}

void SizeIsEmpty()
{
    int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    size_t i = 0, tab_size = 4;
    hash_table_t *hash = HashCreate(tab_size, &MatchInt, &HashFunc);

    puts("\nSize and IsEmpty");
    puts("------------------\n");

    test(0, HashSize(hash), "Size before insert = 0");
    test(1, HashIsEmpty(hash), "IsEmpty before insert = 1");

    for (i = 0; i < 16; ++i)
    {
        HashInsert(hash, arr + i);
    }

    test(16, HashSize(hash), "Size after inserts = 16");
    test(0, HashIsEmpty(hash), "IsEmpty after insert = 0");

    HashDestroy(hash);
}

void SimpleDataFindRemove()
{
    int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    size_t i = 0, tab_size = 4;
    hash_table_t *hash = HashCreate(tab_size, &MatchInt, &HashFunc);

    puts("\nFind and remove - simple data");
    puts("-------------------------------\n");

    for (i = 0; i < 16; ++i)
    {
        HashInsert(hash, arr + i);
    }

    test(12, *(int*)HashFind(hash, arr + 11), "Finding simple data");

    HashRemove(hash, arr + 11);

    addresstest(NULL, HashFind(hash, arr + 11), "Looking for removed data");
    HashDestroy(hash);
}

void ComplexDataFindRemove()
{
    student_t arr[9] = { {"Eli", 1402}, {"Greg", 2712}, {"Ben", 503}, {"Gidi", 1109}, {"Andrew", 3012}, {"Daniel", 210}, {"Igal", 1411}, {"Max", 912}, {"Jenny", 2204} };
    student_t *testdata = NULL;
    size_t i = 0, tab_size = 3;
    hash_table_t *hash = HashCreate(tab_size, &MatchWord, &HashFunc);

    puts("\nFind and remove - complex data");
    puts("--------------------------------\n");

    for (i = 0; i < 9; ++i)
    {
        HashInsert(hash, arr + i);
    }

    test(9, HashSize(hash), "Size after inserts = 16");
    test(0, HashIsEmpty(hash), "IsEmpty after insert = 0");
    testdata = HashFind(hash, &(arr[4].name));
    test(3012, testdata->birthday, "Finding complex data");

    HashRemove(hash, arr + 4);

    addresstest(NULL, HashFind(hash, (arr + 4)->name), "Looking for removed data");
    HashDestroy(hash);
}

void ForEach()
{
    student_t arr[9] = { {"Eli", 1402}, {"Greg", 2712}, {"Ben", 503}, {"Gidi", 1109}, {"Andrew", 3012}, {"Daniel", 210}, {"Igal", 1411}, {"Max", 912}, {"Jenny", 2204} };
    student_t *testdata = NULL;
    size_t i = 0, tab_size = 3;
    hash_table_t *hash = HashCreate(tab_size, &MatchWord, &HashFunc);

    puts("\nForEach");
    puts("---------\n");

    for (i = 0; i < 9; ++i)
    {
        HashInsert(hash, arr + i);
    }

    HashForEach(hash, &DataPlusParam, &i);
    testdata = HashFind(hash, &(arr[4].name));
    test(3021, testdata->birthday, "Checking data after modifying it");

    HashDestroy(hash);
}

void MultipleElements()
{
    int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    size_t i = 0, tab_size = 4;
    hash_table_t *hash = HashCreate(tab_size, &MatchInt, &HashFuncInt);

    puts("\nFind and remove - 5 elements in each dlist");
    puts("--------------------------------------------\n");

    for (i = 0; i < 16; ++i)
    {
        HashInsert(hash, arr + i);
    }

    test(10, *(int*)HashFind(hash, arr + 9), "Finding data");

    HashRemove(hash, arr + 9);

    addresstest(NULL, HashFind(hash, arr + 9), "Looking for removed data");
    HashDestroy(hash);
}

void Dictionary()
{
	char *buffer = NULL;
    char copy[40];
	char str[40];
	size_t i = 0;
	hash_table_t *hash = HashCreate(TABLE_SIZE, &MatchWord, &HashFunc);
    FILE *fptr = NULL;

    puts("\nDictionary");
    puts("------------\n");

	buffer = (char*)calloc(102400, 40);
	if (NULL == buffer)
	{
		printf("malloc failed");
	}
    memset(str, '\0', sizeof(str));
    strcpy(str, "House");

	fptr = fopen("dict.txt", "r");
	if (NULL == fptr)
	{
		printf("File open failure");
        exit(-1);
	}

	while (!feof(fptr))
	{
/*        memset(buffer, '\0', sizeof(buffer));
*/		fgets(buffer + (i * 40), 40, fptr);
        memset(copy, '\0', sizeof(copy));
        strncpy(copy, buffer + (i * 40) , 40);
   		HashInsert(hash, buffer + (i * 40));
		++i;
	}
	test("House", HashFind(hash, str), "Found House?");
/*    addresstest(NULL, HashFind(hash, copy), "Found House?");
	printf("%s", copy);
*/
	free(buffer);
	fclose(fptr);
	HashDestroy(hash);
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
    ((student_t*)data)->birthday += *(int*)param;

	return 0;
}

static int MatchInt(const void *data, const void *param)
{
	return (*(int*)data == *(int*)param);
}

size_t HashFunc(const void *data)
{
	int hash = 401;

	while (*(char *)data != '\0')
	{
		hash = ((hash << 4) + *(int *)(data)) % TABLE_SIZE;
		data = (char *)data + 1;
	}

	return hash ;
}

size_t HashFuncInt(const void *data)
{
	return (*(size_t*)data) % 3;
}
static int MatchWord(const void *data, const void *param)
{
	return (0 == strcmp((char*)data, (char*)param));
}

