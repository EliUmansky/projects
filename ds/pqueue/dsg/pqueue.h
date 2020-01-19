/*************************
     Priority Queue
      Header File
    Author : OL80
      Date : 10/12/2019           
*************************/

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stddef.h> /* size_t */
#include "sortlist.h" /* sorted_list_t */

typedef struct p_queue p_queue_t;
typedef int (*compare_func)(const void *new_data, const void *data, void *param);
typedef int (*is_matched_func)(const void *to_be_matched, const void *param);

/* - creates a new priority queue and returns a pointer to it -------------- */
/* - compare must not be NULL ---------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
p_queue_t *PQCreate(compare_func compare, void *param);

/* - destroy an existing priority queue ------------------------------------ */
/* - pqueue must not be NULL ----------------------------------------------- */
/* - time complexity O(n) -------------------------------------------------- */
void PQDestroy(p_queue_t *pqueue);

/* - returns 1 if the priority queue is empty and 0 if it is not ----------- */
/* - pqueue must not be NULL ----------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
int PQIsEmpty(const p_queue_t *pqueue);

/* - returns the number of elements in the priority queue ------------------ */
/* - pqueue must not be NULL ----------------------------------------------- */
/* - time complexity O(n) -------------------------------------------------- */
size_t PQSize(const p_queue_t *pqueue);

/* - inserts data as a new element into the priority queue ----------------- */
/* - data must not be NULL and be comparable by the queue's compare_func --- */
/* - pqueue must not be NULL ----------------------------------------------- */
/* - time complexity O(n) -------------------------------------------------- */
int PQEnqueue(p_queue_t *pqueue, void *data);

/* - removes the first element of the priority queue and returns it's data - */ 
/* - pqueue must not be NULL ----------------------------------------------- */
/* - pqueue must not be empty ---------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
void *PQDequeue(p_queue_t *pqueue);

/* - returns the data of the first element in the priority queue ----------- */
/* - pqueue must not be NULL ----------------------------------------------- */
/* - pqueue must not be empty ---------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
void *PQPeek(const p_queue_t *pqueue);

/* - removes all the elements from the priority queue ---------------------- */
/* - pqueue must not be NULL ----------------------------------------------- */
/* - time complexity O(n) -------------------------------------------------- */
void PQClear(p_queue_t *pqueue);

/* - erases the first appearance of the element with matched data ---------- */ 
/* - pqueue must not be NULL ----------------------------------------------- */
/* - is_match must not be NULL --------------------------------------------- */
/* - time complexity O(n) -------------------------------------------------- */
void PQErase(p_queue_t *pqueue, is_matched_func is_match, void *param);

#endif /* PRIORITY_QUEUE_H */
