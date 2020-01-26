/******************************************************************************/
/*          Author: Eli Umansky || Reviewer: ♛  Max Zazumin Da King ♛            */
/******************************************************************************/
/*                   Exercise: Heapsort || Date: 23.01.20                     */
/******************************************************************************/

#include <limits.h> /* ULONG_MAX */
#include <string.h> /* memcpy */

#define MAX_OF_SIZE_T (ULONG_MAX)

void HeapSort(int *arr, size_t arr_len);
static void Build(int *arr, size_t arr_len, size_t parent_index);
static void Sort(int *arr, size_t arr_len);
static void HeapifyDown(int *arr, size_t parent_index, size_t arr_len);
static size_t GetBiggestChildIndex(int *arr, size_t parent_index, size_t arr_len);
static size_t GetParentIndex(size_t index);
static void SwapElements(int *element, int *parent_of_element);
static int IsLeaf(size_t index, size_t arr_len);
	
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

