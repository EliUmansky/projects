/*************************
         Queue
      Header File
    Author : OL80
      Date : 02/12/2019           
*************************/

#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue queue_t;

#include <stddef.h> /* size_t */

/* - creates a queue and returns a pointer to it --------------------------- */
queue_t *QueCreate(void);

/* - destroys a queue ------------------------------------------------------ */
/* - queue must not be NULL ------------------------------------------------ */
void QueDestroy(queue_t *queue);

/* - insert data to the end of the queue ----------------------------------- */
/* - returns 0 on success and 1 on memory failure -------------------------- */
/* - queue must not be NULL ------------------------------------------------ */
int QueEnqueue(queue_t *queue, const void *data);

/* - removes the element at the front of the queue ------------------------- */
/* - queue must not be NULL ------------------------------------------------ */
void QueDequeue(queue_t *queue);

/* - returnes the data in the element at the front of the queue ------------ */
/* - queue must not be NULL ------------------------------------------------ */
void *QuePeek(const queue_t *queue);

/* - returnes 1 if the queue is empty and 0 if it is not ------------------- */
/* - queue must not be NULL ------------------------------------------------ */
int QueIsEmpty(const queue_t *queue);

/* - returnes the number of elements currently in the queue ---------------- */
/* - queue must not be NULL ------------------------------------------------ */
size_t QueSize(const queue_t *queue);

/* - appends the src queue to the end of the dest queue -------------------- */
/* - returns a pointer to the appended queue ------------------------------- */
/* - dest must not be NULL ------------------------------------------------- */
/* - src must not be NULL -------------------------------------------------- */
queue_t *QueAppend(queue_t *dest, queue_t *src);

#endif /* QUEUE_H */
