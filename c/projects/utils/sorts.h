/*************************
      Sorts Header
      Header File
    Author : Daniel Oren
      Date : 31/12/2019         
*************************/

#ifndef SORTS_FUNCTIONS_H
#define SORTS_FUNCTIONS_H

#include <stdio.h> 
#include <stddef.h>

typedef int (*cmp_func_t)(const void *data_1, const void *data_2);

void BubbleSort(int *arr, int size);
void SelectionSort(int *arr, int size); 
void InsertionSort(int *arr, int size);
void MergeSort(int *arr, size_t size);
void HeapSort(int *arr, size_t arr_len);
void QuickSort(void *arr, size_t num_of_elements, size_t element_size,
			   cmp_func_t cmp_func);

#endif /* SORTS_FUNCTIONS_H */
