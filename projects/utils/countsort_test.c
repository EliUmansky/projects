#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "radixsort.h"

#define SIZE (20)
#define RAND_SIZE (3000)	
#define STR_LEN (10)

typedef struct list
{
	char name[STR_LEN];
	int id;
} list_t;

int GetKey(void *element, void *param);
static int cmpfunc(const void *a, const void *b);
int CountingTest();
int GetKeyPart(int key, size_t digits_in_part, size_t part_num, void *param);
static int pow(size_t exp, size_t base);

int main()
{
	CountingTest();	

	return 0;
}


int CountingTest()
{
	list_t *arr = NULL;
	int *testarr = NULL;
	clock_t start = 0, end = 0;
	double cpu_time_used;
	int i = 0;

	arr = (list_t*)malloc(SIZE * sizeof(list_t));
	testarr = (int*)malloc(SIZE * sizeof(int));

	if (NULL == arr || NULL == testarr)
	{
		return 1;
	}
	
	for (i = 0; i < SIZE; ++i)
	{
		sprintf(arr[i].name, "Student%d", i + 1);
	
		arr[i].id = rand() % RAND_SIZE;
		testarr[i] = arr[i].id;
	}
/*
	for (i = 0; i < SIZE; ++i)
	{
		printf("%s %d  ", arr[i].name, arr[i].id);
	}*/
	start = clock();
	qsort(testarr, SIZE, sizeof(int), cmpfunc);
	end = clock();
	cpu_time_used = (double)(end - start) / CLOCKS_PER_SEC;

	printf("Time for qsort is %f\n", cpu_time_used);


	start = clock();
	CountingSort(arr, SIZE, sizeof(list_t), 1, 1, &GetKey, NULL, GetKeyPart, NULL);
	CountingSort(arr, SIZE, sizeof(list_t), 1, 2, &GetKey, NULL, GetKeyPart, NULL);
	end = clock();
	cpu_time_used = (double)(end - start) / CLOCKS_PER_SEC;

	printf("Time for CountingSort is %f\n", cpu_time_used);

	for (i = 0; i < SIZE; ++i)
	{
		if (testarr[i] != arr[i].id)
		{
			i = 0;
			break;
		}
	}

	(0 == i) ? printf("Bad algorithm!\n\n") : printf("Good algorithm!\n\n");
	
	free(arr);
	free(testarr);

	return 0;
}
int GetKey(void *element, void *param)
{
	(void)param;

	return ((list_t*)element)->id;
}

static int cmpfunc(const void *a, const void *b)
{
	return (*(int*)a - *(int*)b);
}

int GetKeyPart(int key, size_t digits_in_part, size_t part_num, void *param)
{
	(void)param;	

	return (key / pow(digits_in_part * (part_num - 1), 10)) % pow(digits_in_part, 10);
}

static int pow(size_t exp, size_t base)
{
	size_t res = 1;	

	while (0 < exp)
	{
		res *= base;
		--exp;
	}
	
	return (int)res;
}
