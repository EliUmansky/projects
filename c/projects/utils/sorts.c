
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <limits.h> /* ULONG_MAX */

#include "sorts.h" /* bubble sort */

#define MIN_OF(x, y) (((x) < (y)) ? (x) : (y))
#define MAX_OF_SIZE_T (ULONG_MAX)

/*----------------------------------------------------------static functions--*/
static void Swap(int *num1, int *num2);
static void Merge(int arr1[], int arr2[], size_t len1, size_t len2);
static void Build(int *arr, size_t arr_len, size_t parent_index);
static void Sort(int *arr, size_t arr_len);
static void HeapifyDown(int *arr, size_t parent_index, size_t arr_len);
static size_t GetBiggestChildIndex(int *arr, size_t parent_index, size_t arr_len);
static size_t GetParentIndex(size_t index);
static void SwapElements(int *element, int *parent_of_element);
static int IsLeaf(size_t index, size_t arr_len);
static void RecQuickSort(char *start, char *last_index, size_t nitems,
										size_t element_size, cmp_func_t cmp_func);
static void SwapVoid(void *data1, void *data2, size_t element_size);

/*----------------------------------------------------------------------------*/
void BubbleSort(int *arr, int size)
{
	int i = 0, j = 0;
	int flag = 0;

	assert(NULL != arr);

	for(i = 0; i < size - 1; ++i)
	{
		flag = 0;
		for(j = 0; j < (size - 1 - i); ++j)
		{
			if(arr[j] > arr[j+1])
			{
				Swap((arr + j), (arr + j + 1));
				flag = 1;
			}
		}
		if (!flag)
		{
			return;
		}
	}
}

/*----------------------------------------------------------------------------*/
static void Swap(int *num1, int *num2)
{
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}
/*----------------------------------------------------------------------------*/
void SelectionSort(int *arr, int size)
{
	int i = 0, j = 0;
	int min_index = 0, min = 0;

	assert(NULL != arr);

	for(i = 0; i < size - 1; ++i)
	{
		min_index = i;
		min = arr[i];
		for(j = i + 1; j < size; ++j)
		{
			if(min > arr[j])
			{
				min = arr[j];
				min_index = j;
			}
		}
		Swap((arr + i), (arr + min_index));
	}
}

/*----------------------------------------------------------------------------*/
void InsertionSort(int *arr, int size)
{
	int i = 0;
	int *runner = NULL;
	int *fix_runner = NULL;
	int temp_value = 0;

	assert(NULL != arr);

	runner = arr + 1;
	for(i = 1; i < size; ++i)
	{
		temp_value = *runner;
		fix_runner = runner - 1;
		while(temp_value < *fix_runner && (arr <= fix_runner))
		{
			*(fix_runner + 1) = *fix_runner;
			--fix_runner;
		}
		*(fix_runner + 1) = temp_value;
		++runner;
	}
}

/*----------------------------------------------------------------------------*/
void MergeSort(int arr[], size_t size)
{
	size_t len1 = size / 2;
	size_t len2 = size - len1;
	if (1 == size)
	{
		return;
	}

	MergeSort(arr, len1);
	MergeSort(arr + len1, len2);
	Merge(arr, arr + len1, len1, len2);
}

static void Merge(int arr1[], int arr2[], size_t len1, size_t len2)
{
	int *help_arr = (int*)malloc((len1 + len2) * sizeof(int));
	int *runner1 = arr1, *runner2 = arr2, *help_runner = help_arr;
	
	while (runner1 < arr1 + len1 && runner2 < arr2 + len2)
	{
		if (*runner1 <= *runner2)
		{
			*help_runner = *runner1;
			++help_runner;
			++runner1;
		}
		else
		{
			*help_runner = *runner2;
			++help_runner;
			++runner2;
		}
	}

	while (runner1 < arr1 + len1)
	{
		*help_runner = *runner1;
		++help_runner;
		++runner1;
	}

	while (runner2 < arr2 + len2)
	{
		*help_runner = *runner2;
		++help_runner;
		++runner2;
	}

	memcpy(arr1, help_arr, len1 * sizeof(int));
	memcpy(arr2, help_arr + len1, len2 * sizeof(int));
	free(help_arr);
}

/*----------------------------------------------------------------------------*/
void HeapSort(int *arr, size_t arr_len)
{
	Build(arr, arr_len, GetParentIndex(arr_len - 1));
	Sort(arr, arr_len);
}

void Build(int *arr, size_t arr_len, size_t parent_index)
{
	for (; MAX_OF_SIZE_T != parent_index; --parent_index)
	{
		HeapifyDown(arr, parent_index, arr_len);
	}
}

void Sort(int *arr, size_t arr_len)
{
	while (0 < arr_len)
	{	
		SwapElements(&arr[0], &arr[arr_len - 1]);
		--arr_len;
		HeapifyDown(arr, 0, arr_len);
	}
}

static void HeapifyDown(int *arr, size_t parent_index, size_t arr_len)
{
	size_t biggest_child_index = 0;
	
	if (IsLeaf(parent_index , arr_len))
	{		
		return;
	}

	biggest_child_index = GetBiggestChildIndex(arr, parent_index, arr_len);

	if (arr[biggest_child_index] > arr[parent_index])
	{
		SwapElements(&arr[biggest_child_index], &arr[parent_index]);
		HeapifyDown(arr, biggest_child_index, arr_len);
	}
}

static size_t GetBiggestChildIndex(int *arr, size_t parent_index, size_t arr_len)
{
	size_t child_l_index = (parent_index * 2) + 1;
	size_t child_r_index = (parent_index * 2) + 2;

	if (child_r_index == arr_len)
	{
		return child_l_index;
	}
	else
	{
		return (arr[child_r_index] < arr[child_l_index])
			   ? child_l_index : child_r_index;
	}
}

static size_t GetParentIndex(size_t index)
{
	return index - (index / 2) - 1;
}

static void SwapElements(int *element, int *parent_of_element)
{
	int data_holder = 0;

	memcpy(&data_holder, element, sizeof(int));
	memcpy(element, parent_of_element, sizeof(int));
	memcpy(parent_of_element, &data_holder, sizeof(int));
}

static int IsLeaf(size_t index, size_t arr_len)
{
	return ((index * 2) + 1) >= arr_len;
}

/*----------------------------------------------------------------------------*/
void QuickSort(void *arr, size_t num_of_elements, size_t element_size,
			   cmp_func_t cmp_func)
{
	char *start = arr;
	char *end = (char*)arr + (num_of_elements - 1) * element_size;

	RecQuickSort(start, end, num_of_elements, element_size, cmp_func);
}

void RecQuickSort(char *start, char *end, size_t num_of_elements,
				  size_t element_size, cmp_func_t cmp_func)
{
	char *left = start;
	char *right = end;
	char *pivot = left + ((num_of_elements / 2) * element_size);

	if (start >= end)
	{
		return;
	}

	SwapVoid(right, pivot, element_size);

	while (left < right)
	{
		while (0 > cmp_func(left, end) && left < end)
		{
			left = (char*)left + element_size;
		}
		while (0 <= cmp_func(right, end) && right > start)
		{
			right = (char*)right - element_size;
		}
        if (left < right)
		{
		    SwapVoid(left, right, element_size);
		}
	}

	SwapVoid(left, end, element_size);

	pivot = left;	

	RecQuickSort(start, (char*)pivot - element_size, 
	((size_t)pivot - (size_t)start) / element_size, element_size, cmp_func);
	RecQuickSort((char*)pivot + element_size, end,
	((size_t)end - (size_t)pivot) / element_size, element_size, cmp_func);
}

static void SwapVoid(void *data1, void *data2, size_t element_size)
{
	void *data_holder = (void*)malloc(element_size);
	if (NULL == temp)
	{
		return;
	}

	memcpy(data_holder, data1, element_size);
	memcpy(data1, data2, element_size);
	memcpy(data2, data_holder, element_size);

	free(temp);
}

