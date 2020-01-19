/*************************
      Sorted List
      Header File
    Author : OL80
      Date : 08/12/2019     
*************************/

#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stddef.h> /* size_t */

#include "dlist.h"

typedef struct sorted_list sorted_list_t;
typedef int (*is_before_func)(const void *new_data, const void *data, 
																void *param);

typedef struct sorted_list_iter
{
	dlist_iter_t internal_iter;
}sorted_list_iter_t;

/* - creates a new sorted list and returns a pointer to it ----------------- */
/* - is_before must be a function of sort_rule_t type that is able to ------ */
/* - compare two pieces of data based on the desired sorting criteria ------ */
sorted_list_t *SortedListCreate(is_before_func is_before, void *param);

/* - destroyes a sorted list ----------------------------------------------- */
/* - sorted_list must not be NULL ------------------------------------------ */
void SortedListDestroy(sorted_list_t *sorted_list);

/* - inserts data into it's appropriate place in the list ------------------ */
/* - sorted_list must not be NULL ------------------------------------------ */
sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void *data);

/* - returns the number of elements in the sorted list --------------------- */
/* - sorted_list must not be NULL ------------------------------------------ */
size_t SortedListSize(const sorted_list_t *sorted_list);

/* - returns an iterator to the begining of the sorted list ---------------- */
/* - sorted_list must not be NULL ------------------------------------------ */
sorted_list_iter_t SortedListBegin(const sorted_list_t *sorted_list);

/* - returns an iterator to the end of the sorted list --------------------- */ 
/* - sorted_list must not be NULL ------------------------------------------ */
sorted_list_iter_t SortedListEnd(const sorted_list_t *sorted_list);

/* - returns an iterator to the element after sorted_iter ------------------ */
sorted_list_iter_t SortedListNext(sorted_list_iter_t curr);

/* - returns an iterator to the element before sorted_iter ----------------- */
sorted_list_iter_t SortedListPrev(sorted_list_iter_t curr);

/* - returns 1 if sorted_list is empty and 0 if it is not ------------------ */
/* - sorted_list must not be NULL ------------------------------------------ */
int SortedListIsEmpty(const sorted_list_t *sorted_list);

/* - removes the element from a sorted list pointed by the iterator -------- */
sorted_list_iter_t SortedListRemove(sorted_list_iter_t sorted_iter);

/* - returns the data of the first element in sorted_list and removes it --- */
/* - sorted_list must not be NULL ------------------------------------------ */
void *SortedListPopFront(sorted_list_t *sorted_list);

/* - returns the data of the last element in sorted_list and removes it ---- */
/* - sorted_list must not be NULL ------------------------------------------ */
void *SortedListPopBack(sorted_list_t *sorted_list);

/* - applies action to all the elements from start to end ------------------ */
/* - MUST NOT be used to change the sorting criteria ----------------------- */
/* - end must follow start as an iterator in the same list ----------------- */
int SortedListForEach(sorted_list_iter_t start, sorted_list_iter_t end, 
				action_func action, void *param);

/* - returns an iterator to the first element matchin param as ------------- */
/* - evaluated by is_matched ----------------------------------------------- */
/* - end must follow start as an iterator in the same list ----------------- */
sorted_list_iter_t SortedListFindIf(sorted_list_iter_t start,
			 sorted_list_iter_t end, match_func is_matched, const void *param);

/* - returns the data stored in the element pointed by param --------------- */
void *SortedListGetData(sorted_list_iter_t sorted_iter);

/* - returns 1 if sorted_iter_1 and sorted_iter_2 point to the same element  */
/* - returns 0 is not ------------------------------------------------------ */
int SortedListIsSameIter(sorted_list_iter_t sorted_iter_1,
											 sorted_list_iter_t sorted_iter_2);

/* - merges a section of a list to a destenation list ---------------------- */
/* - last must follow first as an iterator in the same list ---------------- */
void SortedListMerge(sorted_list_t *dest, sorted_list_t *src);

/* - returns an iterator to the element that holds the data if exists else - */
/* - returns end ----------------------------------------------------------- */
/* - sorted_list must not be NULL ------------------------------------------ */
sorted_list_iter_t SortedListFind(sorted_list_t *sorted_list, 
			sorted_list_iter_t start, sorted_list_iter_t end, const void *data);

#endif /* SORTED_LIST_H */
