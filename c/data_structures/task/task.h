/*************************
     Scheduler Task
      Header File
    Author : OL80
      Date : 11/12/2019        
*************************/

#ifndef SCHEDULER_TASK_H
#define SCHEDULER_TASK_H

#include <stddef.h> /* size_t */

#include "sched.h" /* task_action_func */
#include "uid.h" /* ilrd_uid_t */

typedef struct task task_t;

/* - creates a new task and returns a pointer to it ------------------------ */
/* - if interval_in_sec is 0, the task will be executed only once ---------- */
/* - action_func must not be NULL ------------------------------------------ */
/* - destroy_func must not be NULL ----------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
task_t *TaskCreate(task_action_func action_func, void *param,
		 		   size_t interval_in_sec, task_destroy_func destroy_func);

/* - destroys an existing task --------------------------------------------- */
/* - task must not be NULL ------------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
void TaskDestroy(task_t *task);

/* - returns a pointer to the UID of a task -------------------------------- */
/* - task must not be NULL ------------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
ilrd_uid_t *TaskGetUid(task_t *task);

/* - returns the action func of a task ------------------------------------- */
/* - task must not be NULL ------------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
size_t TaskExecuteAction(task_t *task);

/* - sets the time to run of a function to current time + new_delay -------- */
/* - task must not be NULL ------------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
void TaskSetTimeToRun(task_t *task, size_t new_delay);

/* - returns the time_to_run of a task ------------------------------------- */
/* - task must not be NULL ------------------------------------------------- */
/* - time complexity O(1) -------------------------------------------------- */
time_t TaskGetTimeToRun(task_t *task);

#endif /* SCHEDULER_TASK_H */

