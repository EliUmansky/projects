/*************************
    Searching Algorithms
      Header File
      Date : 26/01/2019           
*************************/

#ifndef SEARCH_H
#define SEARCH_H

#include <stdlib.h> /* size_t */

typedef int (*cmp_func_t)(const void *a, const void *b);

void *RecBSearch(const void *base, size_t nmemb, size_t size, 
									cmp_func_t cmp_func, const void *key);

void *BSearch(const void *base, size_t nmemb, size_t size, 
									cmp_func_t cmp_func, const void *key);

void *JumpSearch(const void *base, size_t nmemb, size_t size, 
									cmp_func_t cmp_func, const void *key);
		 
#endif /* SEARCH_H */

