
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <time.h>/* time_t*/

#include "sorts.h" /* bubble sort */

#define SMALL_ARR_SIZE 100
#define BIG_ARR_SIZE 10000

void BCheckSimpleSort();
void BCheckAdvancedSort();
void SCheckSimpleSort();
void SCheckAdvancedSort();
void ICheckSimpleSort();
void ICheckAdvancedSort();
void CCheckSimpleMergeSort();
void ICheckAdvancedMergeSort();
void ICheckAdvancedHeapSort();
void CheckSimpleQuickSort();
void CheckAdvancedQuickSort();

int CompareArr(int *rec, int *exp, int size);
int CompareFunc(const void *a, const void *b);


int main()
{
	BCheckSimpleSort();
	BCheckAdvancedSort();
    SCheckSimpleSort();
	SCheckAdvancedSort();
	ICheckSimpleSort();
	ICheckAdvancedSort();
	CCheckSimpleMergeSort();
	ICheckAdvancedMergeSort();
	ICheckAdvancedHeapSort();
    CheckSimpleQuickSort();
	CheckAdvancedQuickSort();


	return 0;
}

void BCheckSimpleSort()
{
	int *arr = (int *)malloc(sizeof(int) * SMALL_ARR_SIZE);
	int i = 0;

	printf("Arr before sort:\n");
	srand(time(0));
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		printf("arr[i] = %d\n",arr[i]);
	}
	BubbleSort(arr, SMALL_ARR_SIZE);
	printf("\n Arr after sort:\n");
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		printf("arr[i] = %d\n",arr[i]);
	}
}

void BCheckAdvancedSort()
{
	int *arr = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int *cpy_arr1 = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int *cpy_arr2 = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int i = 0;
    clock_t start, end;
    double cpu_time_used;

	for(i = 0; i < BIG_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		cpy_arr1[i] = arr[i];
		cpy_arr2[i] = arr[i];
	}


    start = clock();
	BubbleSort(arr, BIG_ARR_SIZE);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for BubbleSort1 = %f \n",cpu_time_used);


	start = clock();
    BubbleSort(cpy_arr1, BIG_ARR_SIZE);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for BubbleSort2 = %f \n",cpu_time_used);

	start = clock();
    qsort(cpy_arr2, BIG_ARR_SIZE, sizeof(int), &CompareFunc);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for qsort = %f \n",cpu_time_used);
	CompareArr(arr, cpy_arr1, BIG_ARR_SIZE);
}
/*----------------------------------------------------------------------------*/
void SCheckSimpleSort()
{
	int *arr = (int *)malloc(sizeof(int) * SMALL_ARR_SIZE);
	int i = 0;

	printf("Arr before sort:\n");
	srand(time(0));
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		printf("arr[i] = %d\n",arr[i]);
	}
	SelectionSort(arr, SMALL_ARR_SIZE);
	printf("\n Arr after sort:\n");
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		printf("arr[i] = %d\n",arr[i]);
	}
}

void SCheckAdvancedSort()
{
	int *arr = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int *cpy_arr1 = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int i = 0;
    clock_t start, end;
    double cpu_time_used;

	for(i = 0; i < BIG_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		cpy_arr1[i] = arr[i];
	}

    start = clock();
	SelectionSort(arr, BIG_ARR_SIZE);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for selection = %f \n",cpu_time_used);

	start = clock();
    qsort(cpy_arr1, BIG_ARR_SIZE, sizeof(int), &CompareFunc);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for qsort = %f \n",cpu_time_used);

	CompareArr(arr, cpy_arr1, BIG_ARR_SIZE);
}

void ICheckSimpleSort()
{
	int *arr = (int *)malloc(sizeof(int) * SMALL_ARR_SIZE);
	int i = 0;

	printf("Arr before sort:\n");
	srand(time(0));
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		printf("arr[i] = %d\n",arr[i]);
	}
	InsertionSort(arr, SMALL_ARR_SIZE);
	printf("\n Arr after sort:\n");
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		printf("arr[i] = %d\n",arr[i]);
	}
}

void ICheckAdvancedSort()
{
	int *arr = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int *cpy_arr1 = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int i = 0;
    clock_t start, end;
    double cpu_time_used;

	for(i = 0; i < BIG_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		cpy_arr1[i] = arr[i];
	}

    start = clock();
	InsertionSort(arr, BIG_ARR_SIZE);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for Insertion = %f \n",cpu_time_used);

	start = clock();
    qsort(cpy_arr1, BIG_ARR_SIZE, sizeof(int), &CompareFunc);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for qsort = %f \n",cpu_time_used);

	CompareArr(arr, cpy_arr1, BIG_ARR_SIZE);
}

void CCheckSimpleMergeSort()
{
	int *arr = (int *)malloc(sizeof(int) * SMALL_ARR_SIZE);
	int i = 0;

	printf("Arr before sort:\n");
	srand(time(0));
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		arr[i] = (rand() % 15) + 5;
		printf("arr[i] = %d\n",arr[i]);
	}
	MergeSort(arr, SMALL_ARR_SIZE);
	printf("\n Arr after sort:\n");
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		printf("arr[i] = %d\n",arr[i]);
	}
	free(arr);
}

void ICheckAdvancedMergeSort()
{
	int *arr = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int *cpy_arr1 = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int i = 0;
    clock_t start, end;
    double cpu_time_used;

	for(i = 0; i < BIG_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		cpy_arr1[i] = arr[i];
	}

    start = clock();
	MergeSort(arr, BIG_ARR_SIZE);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for Merge Sort = %f \n",cpu_time_used);

	start = clock();
    qsort(cpy_arr1, BIG_ARR_SIZE, sizeof(int), &CompareFunc);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for qsort = %f \n",cpu_time_used);

	CompareArr(arr, cpy_arr1, BIG_ARR_SIZE);

	free(arr);
	free(cpy_arr1);

}

void ICheckAdvancedHeapSort()
{
	int *arr = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int *cpy_arr1 = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int i = 0;
    clock_t start, end;
    double cpu_time_used;

	for(i = 0; i < BIG_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		cpy_arr1[i] = arr[i];
	}

    start = clock();
	HeapSort(arr, BIG_ARR_SIZE);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for Heap Sort = %f \n",cpu_time_used);

	start = clock();
    qsort(cpy_arr1, BIG_ARR_SIZE, sizeof(int), &CompareFunc);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for qsort = %f \n",cpu_time_used);

	CompareArr(arr, cpy_arr1, BIG_ARR_SIZE);

	free(arr);
	free(cpy_arr1);

}

/*void CCheckSimpleHeapSort()
{
	int arr[8] = {4, 6, 1, 7, 3, 2, 8, 5};
	int i = 0;

	printf("Arr before sort:\n");

	for(i = 0; i < 8; ++i)
	{
		printf("arr[i] = %d\n",arr[i]);
	}
	HeapSort(arr, 8);
	printf("\n Arr after sort:\n");
	for(i = 0; i < 8; ++i)
	{
		printf("arr[i] = %d\n",arr[i]);
	}
}*/

/*void CCheckSimpleHeapSort()
{
	int i = 0;
	int arr[8] = {74, 65, 80, 90, 111, 79, 94,112};;

	printf("Arr before sort:\n");

	HeapSort(arr, SMALL_ARR_SIZE);
	printf("\n Arr after sort:\n");
	for(i = 0; i < 8; ++i)
	{
		printf("arr[%d] = %d\n", i, arr[i]);
	}

}*/
void CheckSimpleQuickSort()
{
	int *arr = (int *)malloc(sizeof(int) * SMALL_ARR_SIZE);
	int i = 0;

	printf("Arr before sort:\n");
	srand(time(0));
	for (i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		*(arr + i) = rand() % 100 ;
		printf("%d ", *(arr + i));
	}

	puts("");

	QuickSort(arr, SMALL_ARR_SIZE, sizeof(int), &CompareFunc);
	printf("\nArr after sort:\n");
	for(i = 0; i < SMALL_ARR_SIZE; ++i)
	{
		printf("%d ",*(arr + i));
	}

	free(arr);
}

void CheckAdvancedQuickSort()
{
	int *arr = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int *cpy_arr1 = (int *)malloc(sizeof(int) * BIG_ARR_SIZE);
	int i = 0;
    clock_t start, end;
    double cpu_time_used;
	srand(time(0));
	for(i = 0; i < BIG_ARR_SIZE; ++i)
	{
		arr[i] = rand() % 100;
		cpy_arr1[i] = arr[i];
	}

    start = clock();
	QuickSort(arr, BIG_ARR_SIZE, sizeof(int), &CompareFunc);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for Quick Sort = %f \n",cpu_time_used);

	start = clock();
    qsort(cpy_arr1, BIG_ARR_SIZE, sizeof(int), &CompareFunc);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time took for qsort = %f \n",cpu_time_used);

	CompareArr(arr, cpy_arr1, BIG_ARR_SIZE);

	free(arr);
	free(cpy_arr1);

}

int CompareArr(int *rec, int *exp, int size)
{
	int i = 0;

	for(i = 0; i < size; ++i)
	{
		if(rec[i] != exp[i])
		{
			printf("NOT GOOD!\n");
			return 0;
		}
	}
	printf("PERFECT SORT!\n");
	return 1;
}


int CompareFunc(const void *a, const void *b)
{
	return (*(int*)a - *(int *)b);
}
