/*************************
   Doubly Linked List
      Header File
    Author : OL80
      Date : 04/12/2019           
*************************/

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

typedef struct dlist dlist_t;
typedef struct dlist_node *dlist_iter_t;
typedef int (*action_func)(void *data, void *param);
typedef int (*match_func)(const void *data, const void *param);

#define DUMMY_DATA (0xbadcabab) 

#include <stddef.h> /* size_t */

/* - creates a new doubly linked list and returns a pointer to it ---------- */
dlist_t *DListCreate(void);

/* - destorys an existing doubly linked list ------------------------------- */
/* - dlist must not be NULL ------------------------------------------------ */
void DListDestroy(dlist_t *dlist);

/* - returns an iterators set to the first element of the linked lisk ------ */
/* - dlist must not be NULL ------------------------------------------------ */
dlist_iter_t DListBegin(const dlist_t *dlist);

/* - returns an iterator set to the end of the linked list ----------------- */
/* - ! such itarator should ne be DListRemoved ! --------------------------- */
/* - dlist must not be NULL ------------------------------------------------ */
dlist_iter_t DListEnd(const dlist_t *dlist);

/* - moves the iterator one step back, it must not point to head ----------- */
dlist_iter_t DListPrev(dlist_iter_t current);

/* - moves the iterator one step forward, it must not point to tail -------- */
dlist_iter_t DListNext(dlist_iter_t current);

/* - returns the nuber of all the elements in the list --------------------- */
/* - dlist must not be NULL ------------------------------------------------ */
size_t DListSize(const dlist_t *dlist);

/* - returns 1 if the node is empty and 0 if it is not --------------------- */
/* - dlist must not be NULL ------------------------------------------------ */
int DListIsEmpty(const dlist_t *dlist);

/* - returns 1 if both iterators point to the same place and 0 if not ------ */
int DListIsSameIterator(dlist_iter_t diter_1, dlist_iter_t diter_2); 

/* - return a pointer to the data in the element indicated by the iterator - */
void *DListGetData(dlist_iter_t diter);

/* - inserts an element in the list, before the iterator ------------------- */
/* - returns an itarator set to the newely inserted element ---------------- */
dlist_iter_t DListInsertBefore(dlist_t *dlist, dlist_iter_t where, void *data);

/* - removes the element the iterator is on -------------------------------- */
/* - returns an iterator set to the next element --------------------------- */
dlist_iter_t DListRemove(dlist_iter_t diter);

/* - creates a new element using data and pushes it to the front of dlist -- */
/* - dlist must not be NULL ------------------------------------------------ */
dlist_iter_t DListPushFront(dlist_t *dlist, void *data);

/* - creates a new element using data and pushes it to the back of dlist --- */
/* - dlist must not be NULL ------------------------------------------------ */
dlist_iter_t DListPushBack(dlist_t *dlist, void *data);

/* - removes the first element of the list and return it's data ------------ */
/* - dlist must not be NULL ------------------------------------------------ */
void *DListPopFront(dlist_t *dlist);

/* - removes the last element of the list and return it's data ------------- */
/* - dlist must not be NULL ------------------------------------------------ */
void *DListPopBack(dlist_t *dlist);

/* - appends the chain from first to last to where ------------------------- */
/* - returns an iterator set to where -------------------------------------- */
dlist_iter_t DListSplice(dlist_iter_t where, dlist_iter_t first, 
						 dlist_iter_t last);

/* - performs action on all the elements from start (inclusive) ------------ */
/* - to end (exclusive) ---------------------------------------------------- */
/* - if one such action fails, the function will abort and return an error - */
int DListForEach(dlist_iter_t start, dlist_iter_t end, action_func action, 
				 void *param);

/* - returns the first element that matches param starting from ------------ */
/* - start (inclusive) and up to end (exclusive) --------------------------- */
/* - is_match must be a function that can check such a match --------------- */
/* - is_match should return 1 on a hit and 0 on a miss --------------------- */
dlist_iter_t DListFind(dlist_iter_t start, dlist_iter_t end, 
					   match_func is_match, const void *param);

#endif /* DOUBLY_LINKED_LIST_H */
