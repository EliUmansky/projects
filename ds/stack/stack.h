/***********************/
/*	   BitsArray.h	   */
/*  Date: 25.11.2019   */
/*		  OL80		   */
/***********************/

#ifndef STACK_H
#define STACK_H

#include <stdio.h>

typedef struct stackstruct stack_t;


/* create a new stack designed to hold stack_size elemnts of element_size */
stack_t *StackCreate(size_t num_of_elements, size_t element_size);

/* - destroys stack stack -------------------------------------------------- */
/* - stack must not be null ------------------------------------------------ */
void StackDestroy(stack_t *stack);

/* - returns the number of elements currently occupying the stack ---------- */
/* - stack must not be null ------------------------------------------------ */
size_t StackSize(const stack_t *stack);

/* - pushes an element to the top of the stack ----------------------------- */
/* - data must not be null, and be of the element type held in the stack --- */
/* - stack must not be null, and not full ---------------------------------- */
void StackPush(stack_t *stack,const void *data);

/* - pops the top element from the stack ----------------------------------- */
/* - stack must not be null ------------------------------------------------ */
/* - stack must not be null, and not empty --------------------------------- */
void StackPop(stack_t *stack);

/* - returns a pointer to the top element of the stack  ------------------------ */
/* - stack must not be null, and not empty --------------------------------- */
void *StackPeek(const stack_t *stack);

/* - returns 1 if the stack is empty and 0 if not -------------------------- */
/* - stack must not be null ------------------------------------------------ */
int StackIsEmpty(const stack_t *stack);

#endif /* STACK_H */

