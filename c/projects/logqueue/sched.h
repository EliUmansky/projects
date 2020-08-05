/*************************
       Scheduler
      Header File
    Author : OL80
      Date : 11/12/2019           
*************************/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h> /* size_t */

#include "uid.h" /* uid */

typedef struct scheduler scheduler_t;

typedef size_t (*task_action_func)(void *param);
typedef void (*task_destroy_func)(void *param);


/* - creates a new scheduler and returns a pointer to it ------------------- */
/* - time complexity O(1) -------------------------------------------------- */
scheduler_t *SchedCreate();

/* - destroyes an existing scheduler --------------------------------------- */
/* - WARNING: all tesks inside will be lost -------------------------------- */
/* - sched must not be NULL ------------------------------------------------ */
/* - time complexity O(n) -------------------------------------------------- */
void SchedDestroy(scheduler_t *sched);

/* - adds a new task to the scheduler -------------------------------------- */
/* - time to run will be the time the task will be set to run -------------- */
/* - interval_in_seconds will be the interval to repeat the task ----------- */
/* - if interval_in_seconds is set to 0, the task will not repeat ---------- */
/* - action_func would be the action performed with param as an argument --- */
/* - destroy_func would be ran once before task removal -------------------- */
/* - sched must not be NULL ------------------------------------------------ */
/* - time complexity O(n) -------------------------------------------------- */
ilrd_uid_t *SchedAddTask(scheduler_t *sched, task_action_func action_func,
										void *param,
										size_t interval_in_seconds,  
										task_destroy_func destroy_func);

/* - removes a task from the schedualer ------------------------------------ */
/* - sched must not be NULL ------------------------------------------------ */
/* - uid must not be bad_uid ----------------------------------------------- */
void SchedRemoveTask(scheduler_t *sched, ilrd_uid_t *uid);

/* - begins to run a schedualer -------------------------------------------- */
/* - this function will not stop on it's own and must be stopped by -------- */
/* - sending it SchedStop and a task --------------------------------------- */
/* - sched must not be NULL ------------------------------------------------ */
/* - time complexity - depands on the tasks within it ---------------------- */
int SchedRun(scheduler_t *sched);

/* - adds a task to stop the schedualer at a predetermined time ------------ */
/* - should be added to the scheduler before calling SchedRun -------------- */
/* - sched must not be NULL ------------------------------------------------ */
/* - time complexity O(n) -------------------------------------------------- */
void SchedStop(scheduler_t *sched);

/* - returns number of scheduled tasks currently in queue ------------------ */
/* - sched must not be NULL ------------------------------------------------ */
/* - time complexity O(n) -------------------------------------------------- */
size_t SchedSize(const scheduler_t *sched);

/* - returns 1 if there are no tasks in the scheduler ---------------------- */
/* - sched must not be NULL ------------------------------------------------ */
/* - time complexity O(1) -------------------------------------------------- */
int SchedIsEmpty(const scheduler_t *sched);

/* - clears all the tasks in the scheduler --------------------------------- */
/* - sched must not be NULL ------------------------------------------------ */
/* - time complexity O(n) -------------------------------------------------- */
void SchedClear(scheduler_t *sched);

#endif /* SCHEDULER_H */
