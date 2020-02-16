/*************************
   System V Semaphore
      Header File
    Author : OL80
      Date : 12/02/2020        
*************************/

#ifndef SYSTEM_V_SEMAPHORE_H
#define SYSTEM_V_SEMAPHORE_H

#include <stddef.h> /* size_t */

typedef int semid_t;
typedef unsigned short ushort_t;

/* - attempts to create an array of semaphores and returns it's id ---------- */
/* - if the semaphore already exist, this function opens it for append ------ */
/* - to append an existing semaphore, path MUST be the same for both -------- */
/* - init_values_list MAY be NULL. if it is, all inits will be 0 ------------ */
/* - num_of_sems must be greater then 0 ------------------------------------- */
/* - path must not be NULL -------------------------------------------------- */
/* - time complexity O(n) --------------------------------------------------- */
semid_t SysVSemCreate(size_t num_of_sems, const int *init_values_list, 
                      const char *path);

/* - attemps to destroy an existing array of semaphores --------------------- */
/* - the operation succeeds only if attempted from the last proccess -------- */
/* - using the semaphore, but MUST be called on all processes with Create --- */
/* - time complexity O(n) --------------------------------------------------- */
int SysVSemDestroy(semid_t sem_id);

/* - attemps to decrease the semaphore. if the semaphore is already -1 ------ */
/* - the function blocks the proccess untill semaphore increases ------------ */
/* - time complexity O(1) --------------------------------------------------- */
int SysVSemWait(semid_t sem_id, size_t sem_index);

/* - the function blocks the proccess until the semaphore is 0 -------------- */
/* - time complexity O(1) --------------------------------------------------- */
int SysVSemWaitForZero(semid_t sem_id, size_t sem_index);

/* - increases the semaphore by 1 ------------------------------------------- */
/* - time complexity O(1) --------------------------------------------------- */
int SysVSemPost(semid_t sem_id, size_t sem_index);

#endif /* SYSTEM_V_SEMAPHORE_H */
