#include <stdlib.h> /* size_t */
#include <assert.h>

#include "radixsort.h"

int GetKeyPart(int key, size_t digits_in_part, size_t part_num, void *param);
static int power(size_t exp, size_t base);
static int FindMax(void *arr, size_t num_of_elements, size_t element_size,
				   get_key_func_t GetKey, void *get_key_param);

int SortRadix(void *arr, size_t num_of_elements, size_t element_size,
			  size_t digits_in_part, get_key_func_t GetKey, void *param)
{
	int max = 0;
	int part_num = 1, status = 0;
	
	assert (NULL != arr);
	assert (0 < num_of_elements);
	assert (0 < element_size);
	assert (0 < digits_in_part);

	max = FindMax(arr, num_of_elements, element_size, GetKey, param);

	while (0 < max && 0 == status)
	{
		status = CountingSort(arr, num_of_elements, element_size, digits_in_part,
						      part_num, GetKey, NULL, &GetKeyPart, NULL);

		max /= power(digits_in_part, 10); 
		++part_num;
	}

	return status;
}

int GetKeyPart(int key, size_t digits_in_part, size_t part_num, void *param)
{
	(void)param;	

	return (key / power(digits_in_part * (part_num - 1), 10)) % 
			power(digits_in_part, 10);
}

static int power(size_t exp, size_t base)
{
	size_t res = 1;	

	while (0 < exp)
	{
		res *= base;
		--exp;
	}
	
	return (int)res;
}

static int FindMax(void *arr, size_t num_of_elements, size_t element_size,
				   get_key_func_t GetKey, void *get_key_param)
{
	void *runner = arr, *end = (char*)arr + (num_of_elements * element_size);
	int max = GetKey(arr, get_key_param);

	while (runner < end)
	{
		if (GetKey(runner, get_key_param) > max)
		{
			max = GetKey(runner, get_key_param);
		}

		runner = (char*)runner + element_size;
	}
	
	return max;
}
