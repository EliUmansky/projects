/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Ben Shomroni ||| Date 15.12.19  */
/***********************************************************************/
/*      This file contains all the functions of the header task.h      */
/***********************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <time.h> /* time */

#include "sched.h" /* scheduler_t */
#include "task.h" /* task_t */
#include "uid.h" /* ilrd_uid_t */

struct task 
{
	ilrd_uid_t task_id;
	task_action_func action_func;
	void *param;
	time_t time_to_run;
	task_destroy_func destroy_func;
};

/* --------------------------------------------------------- Task Create --- */

task_t *TaskCreate(task_action_func action_func, void *param,
		 		   size_t interval_in_sec, task_destroy_func destroy_func)
{
	task_t *task = NULL;

	assert(NULL != action_func);
	assert(NULL != destroy_func);

	task = (task_t *)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}

	task->task_id = UIDCreate();
	task->action_func = action_func;
	task->param = param;
	task->time_to_run = time(NULL) + interval_in_sec;
	task->destroy_func = destroy_func;

	return task;
}

/* -------------------------------------------------------- Task Destroy --- */

void TaskDestroy(task_t *task)
{
	assert(NULL != task);

	task->destroy_func(task->param);
	free(task);
}

/* -------------------------------------------------------- Task Get Uid --- */

ilrd_uid_t *TaskGetUid(task_t *task)
{
	assert(NULL != task);

	return &(task->task_id);
}

/* ----------------------------------------------------- Task Get Action --- */

size_t TaskExecuteAction(task_t *task)
{
	assert(NULL != task);

	return (task->action_func)(task->param);
}

/* ------------------------------------------------- Task Get Time To Run --- */

time_t TaskGetTimeToRun(task_t *task)
{
	assert(NULL != task);

	return task->time_to_run;
}
/* ------------------------------------------------- Task Set Time To Run --- */
void TaskSetTimeToRun(task_t *task, size_t new_delay)
{
	assert(NULL != task);

	task->time_to_run = new_delay + time(NULL);
}

