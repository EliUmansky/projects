/*************************
   Recursion Excercise
      Header File
    Author : OL80
      Date : 09/01/2020         
*************************/

#ifndef RECURSION_H
#define RECURSION_H

#include <stddef.h> /* size_t */

#include "slist.h" /* slist_node_t */
#include "stack.h" /* stack_t */

size_t Fibonacci(size_t num);
size_t RecFibonacci(size_t num);
size_t RecStrLen(const char *str);
int RecStrCmp(const char *str_a, const char *str_b);
char *RecStrCpy(char *dest, const char *src);
char *RecStrCat(char *dest, const char *src);
char *RecStrStr(const char *haystack, const char *needle);
slist_node_t *RecSlistFlip(slist_node_t *head);
void RecStackSort(stack_t *stack);

#endif /* RECURSION_H */
