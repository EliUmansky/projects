#include <stdlib.h> /* malloc */
#include <string.h> /* mamcpy */
#include <assert.h> /* assert */

#include "countsort.h" /* get_key_func_t */

#define ARR_SIZE ((num_of_elements) * (element_size))
#define ARR_KEY_PART (GetKeyPart(GetKey(arr, get_key_param), digits_in_part, part_num, get_key_part_param))
#define RUNNER_KEY_PART (GetKeyPart(GetKey(runner, get_key_param), digits_in_part, part_num, get_key_part_param))

static void FindMinMax(void *arr, size_t num_of_elements, size_t element_size,
					   size_t digits_in_part, size_t part_num, int *min, int *max,
					   get_key_func_t GetKey, get_key_part_func_t GetKeyPart,
					   void *get_key_param, void *get_key_part_param);
static void SetMap(void *arr, int *map_arr, size_t num_of_elements,
				   size_t element_size, size_t digits_in_part, size_t part_num,
				   get_key_func_t GetKey, get_key_part_func_t GetKeyPart,
				   void *get_key_param, void *get_key_part_param, int range, int min);
static void SortArray(void *arr, int *map_arr, void *sorted_arr, size_t num_of_elements,
				      size_t element_size, size_t digits_in_part, size_t part_num,
					  get_key_func_t GetKey, get_key_part_func_t GetKeyPart,
				   	  void *get_key_param, void *get_key_part_param, int min);
static void CopySortedToArr(void *arr, void *sorted_arr, size_t num_of_elements,
							size_t element_size);

int CountingSort(void *arr, size_t num_of_elements, size_t element_size,
			  size_t digits_in_part, size_t part_num, get_key_func_t GetKey,
			  void *get_key_param, get_key_part_func_t GetKeyPart,
			  void *get_key_part_param)
{
	void *sorted_arr = NULL, *map_arr = NULL;
	int min = 0, max = 0, range = 0;

	assert (NULL != arr);
	assert (0 < num_of_elements);
	assert (0 < element_size);
	assert (0 < digits_in_part);
	assert (0 < part_num);

	sorted_arr = (void*)malloc(ARR_SIZE);
	if (NULL == sorted_arr)
	{
		return -1;
	}

	FindMinMax(arr, num_of_elements, element_size, digits_in_part, part_num,
			   &min, &max, GetKey, GetKeyPart, get_key_param, get_key_part_param);
	range = max - min + 1;

	map_arr = (int*)calloc(range, sizeof(int));
	if (NULL == map_arr)
	{
		return -2;
	}

	SetMap(arr, map_arr, num_of_elements, element_size, digits_in_part, part_num,
		   GetKey, GetKeyPart, get_key_param, get_key_part_param, range, min);
	SortArray(arr, map_arr, sorted_arr, num_of_elements, element_size,
			  digits_in_part, part_num, GetKey, GetKeyPart, get_key_param,
			  get_key_part_param, min);
	CopySortedToArr(arr, sorted_arr, num_of_elements, element_size);

	free(sorted_arr);
	free(map_arr);

	return 0;
}

static void FindMinMax(void *arr, size_t num_of_elements, size_t element_size,
					   size_t digits_in_part, size_t part_num, int *min, int *max,
					   get_key_func_t GetKey, get_key_part_func_t GetKeyPart,
					   void *get_key_param, void *get_key_part_param)
{
	void *runner = arr, *end = (char*)arr + ARR_SIZE;

	*min = ARR_KEY_PART;
	*max = *min;

	while (runner < end)
	{
		if (RUNNER_KEY_PART > *max)
		{
			*max = RUNNER_KEY_PART;
		}
		else if (RUNNER_KEY_PART < *min)
		{
			*min = RUNNER_KEY_PART;
		}

		runner = (char*)runner + element_size;
	}
}

static void SetMap(void *arr, int *map_arr, size_t num_of_elements,
				   size_t element_size, size_t digits_in_part, size_t part_num,
				   get_key_func_t GetKey, get_key_part_func_t GetKeyPart,
				   void *get_key_param, void *get_key_part_param, int range, int min)
{
	void *runner = arr, *end = (char*)arr + ARR_SIZE;
	int *map_runner = map_arr + 1, *map_end = map_arr + range;

	while (runner < end)
	{
		++(*(map_arr + RUNNER_KEY_PART - min));
		runner = (char*)runner + element_size;
	}

	--*map_arr;

	while (map_runner < map_end)
	{
		*map_runner += *(map_runner - 1);
		++map_runner;
	}
}

static void SortArray(void *arr, int *map_arr, void *sorted_arr, size_t num_of_elements,
				      size_t element_size, size_t digits_in_part, size_t part_num,
					  get_key_func_t GetKey, get_key_part_func_t GetKeyPart,
				   	  void *get_key_param, void *get_key_part_param, int min)
{
	void *runner = (char*)arr + ARR_SIZE - element_size;
	size_t sorted_index = 0;
	
	while (runner >= arr)
	{
		sorted_index = *(map_arr + (RUNNER_KEY_PART - min));
		
		memcpy((char*)sorted_arr + (element_size * sorted_index), runner, element_size);
		--*(map_arr + (RUNNER_KEY_PART - min));

		runner = (char*)runner - element_size;
	}
}

static void CopySortedToArr(void *arr, void *sorted_arr, size_t num_of_elements,
							size_t element_size)
{
	void *runner = arr, *sorted_runner = sorted_arr, *end = (char*)arr + ARR_SIZE;

	while (runner < end)
	{
		memcpy(runner, sorted_runner, element_size);

		runner = (char*)runner + element_size;
		sorted_runner = (char*)sorted_runner + element_size;
	}	
}

