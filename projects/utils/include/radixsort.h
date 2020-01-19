/*******************************
    Counting Sort Header File
     Author : Eli Umansky
       Date : 2/1/2020           
********************************/
#include <stdlib.h> /* size_t */

#include "countsort.h"

#ifndef RADIX_H
#define RADIX_H

int SortRadix(void *arr, size_t num_of_elements, size_t element_size,
			  size_t digits_in_part, get_key_func_t get_key_func, void *param);

#endif /* RADIX_H */

