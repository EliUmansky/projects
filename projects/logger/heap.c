/******************************************************************************/
/*              Author: Eli Umansky || Reviewer: Greg Markovsky               */
/******************************************************************************/
/*                      Exercise: Heap || Date: 22.01.20                      */
/******************************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <limits.h> /* ULONG_MAX */

#include "heap.h" /* avl_t */
#include "vector.h" /* d_vector_t */

#define SUCCESS (0)
#define MALLOC_FAIL (-1)
#define DVEC_NUM_OF_ELEMENTS (40)
#define DVEC (heap->dvec)
#define CMP_FUNC (heap->cmp_func)
#define PARAM (heap->param)
#define MAX_OF(x, y) (((x) > (y)) ? (x) : (y))
#define FIRST_INDEX_IN_ARR (0)
#define MAX_OF_SIZE_T (ULONG_MAX)
#define ENOMEM (12)

struct heap
{
	d_vector_t *dvec;
	cmp_func_t cmp_func;
	void *param;
};

int errno = 0;

static void HeapifyUp(heap_t *heap, size_t child_index);
static size_t HeapifyDown(heap_t *heap, size_t parent_index);
static size_t GetParentIndex(size_t index);
static void SwapElements(void **element, void **parent_of_element);
static int IsLeaf(heap_t *heap, size_t index);
static size_t GetSmallestChildIndex(heap_t *heap, size_t parent_index);
static size_t FindIndexOfRemoved(heap_t *heap, const void *data,
								 match_func_t is_matched);
static void RemoveElement(heap_t *heap, size_t index_of_removed);

heap_t *HeapCreate(cmp_func_t cmp_func, void *param)
{
	heap_t *heap = NULL;

	assert (NULL != cmp_func);
	
	heap = (heap_t*)malloc(sizeof(heap_t));
	if (NULL == heap)
	{
		return NULL;
	}

	heap->cmp_func = cmp_func;
	heap->param = param;
	heap->dvec = DvecCreate(sizeof(void*), DVEC_NUM_OF_ELEMENTS);
	if (NULL == heap->dvec)
	{
		free(heap);
		return NULL;
	}

	return heap;
}

void HeapDestroy(heap_t *heap)
{
	assert (NULL != heap);

	DvecDestroy(DVEC);
	free(heap);
}

size_t HeapSize(const heap_t *heap)
{
	assert (NULL != heap);

	return DvecSize(DVEC);
}

int HeapIsEmpty(const heap_t *heap)
{
	assert (NULL != heap);

	return (0 == HeapSize(heap));
}

void *HeapPeek(const heap_t *heap)
{	
	assert (NULL != heap);

	return *(size_t**)DvecGetItemAddress(DVEC, FIRST_INDEX_IN_ARR);
}

void *HeapRemove(heap_t *heap, match_func_t is_matched, const void *data)
{
	size_t index_of_removed = 0;
	void *data_of_removed = NULL;
	
	assert (NULL != heap);

	index_of_removed = FindIndexOfRemoved(heap, data, is_matched);
	if (MAX_OF_SIZE_T == index_of_removed)
	{
		return NULL;
	}
	
	data_of_removed = *(size_t**)DvecGetItemAddress(DVEC, index_of_removed);
	RemoveElement(heap, index_of_removed);

	return data_of_removed;
}

void *HeapPop(heap_t *heap)
{
	size_t index_of_removed = 0;	
	void *data_of_removed = NULL;

	assert (NULL != heap);

	data_of_removed = HeapPeek(heap);
	RemoveElement(heap, index_of_removed);

	return data_of_removed;
}

static void RemoveElement(heap_t *heap, size_t index_of_removed)
{
	void *address_of_removed = DvecGetItemAddress(DVEC, index_of_removed);
	void *last_element_address = DvecGetItemAddress(DVEC, HeapSize(heap) - 1);

	SwapElements(address_of_removed, last_element_address);
	errno = (SUCCESS == DvecPopBack(DVEC)) ? SUCCESS : ENOMEM;

	if (SUCCESS == errno && 0 != HeapSize(heap))
	{
		HeapifyUp(heap, index_of_removed);
		HeapifyDown(heap, index_of_removed);
	}
}

static size_t FindIndexOfRemoved(heap_t *heap, const void *data, 
								 match_func_t is_matched)
{
	size_t index = 0;
	int status = 0;

	for (; index < HeapSize(heap) && 0 == status; ++index)
	{
		status = is_matched(*(size_t**)DvecGetItemAddress(DVEC, index), data);
	}

	return (index >= HeapSize(heap) - 1 && 0 == status) ? MAX_OF_SIZE_T : index - 1;
}

static size_t HeapifyDown(heap_t *heap, size_t parent_index)
{
	size_t smallest_child_index = 0;
	void *smallest_child_address = NULL;
	void *parent_address = DvecGetItemAddress(DVEC, parent_index);
	
	if (IsLeaf(heap, parent_index))
	{		
		return parent_index;
	}

	smallest_child_index = GetSmallestChildIndex(heap, parent_index);
	smallest_child_address = DvecGetItemAddress(DVEC, smallest_child_index);

	if (0 > CMP_FUNC(*(size_t**)smallest_child_address, 
					 *(size_t**)parent_address, PARAM))
	{
		SwapElements(smallest_child_address, parent_address);
	}

	return HeapifyDown(heap, smallest_child_index);
}

static size_t GetSmallestChildIndex(heap_t *heap, size_t parent_index)
{
	size_t child_l_index = (parent_index * 2) + 1;
	size_t child_r_index = (parent_index * 2) + 2;


	if (child_r_index > HeapSize(heap) - 1)
	{
		return child_l_index;
	}
	else
	{
		return (0 > CMP_FUNC(*(size_t**)DvecGetItemAddress(DVEC, child_l_index),
							 *(size_t**)DvecGetItemAddress(DVEC, child_r_index), 
							 PARAM)) ? child_l_index : child_r_index;
	}
}

int HeapPush(heap_t *heap, void *data)
{
	assert (NULL != heap);

	errno = (SUCCESS == DvecPushBack(DVEC, &data)) ? SUCCESS : ENOMEM;
	if (SUCCESS == errno)
	{
		HeapifyUp(heap, HeapSize(heap) - 1);
	}

	return errno;
}

static void HeapifyUp(heap_t *heap, size_t child_index)
{
	size_t parent_index = 0;
	void *child_address = NULL, *parent_address = NULL;

	if (0 == child_index)
	{
		return;
	}
	
	parent_index = GetParentIndex(child_index);
	child_address = DvecGetItemAddress(DVEC, child_index);
	parent_address = DvecGetItemAddress(DVEC, parent_index);

	if (0 > CMP_FUNC(*(size_t**)child_address, 
					 *(size_t**)parent_address, PARAM))
	{
		SwapElements(child_address, parent_address);
		child_index = parent_index;
		HeapifyUp(heap, child_index);
	}
}

static size_t GetParentIndex(size_t index)
{
	return index - (index / 2) - 1;
}

static void SwapElements(void **element, void **parent_of_element)
{
	void *temp = NULL;

	temp = *element;
	*element = *parent_of_element;
	*parent_of_element = temp;
}

static int IsLeaf(heap_t *heap, size_t index)
{
	return ((index * 2) + 1) > HeapSize(heap) - 1;
}

