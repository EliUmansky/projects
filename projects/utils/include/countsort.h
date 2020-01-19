/*******************************
    Radix Sort Header File
     Author : Eli Umansky
       Date : 2/1/2020           
********************************/
#include <stdlib.h> /* size_t */

#ifndef COUNT_H
#define COUNT_H

typedef int(*get_key_func_t)(void *data, void *param);
typedef int(*get_key_part_func_t)(int key, size_t digits_in_part,
			size_t part_num, void *param);

int CountingSort(void *arr, size_t num_of_elements, size_t element_size,
			  size_t digits_in_part, size_t part_num, get_key_func_t GetKey,
			  void *get_key_param, get_key_part_func_t GetKeyPart,
			  void *get_key_part_param);


#endif /* COUNT_H */
