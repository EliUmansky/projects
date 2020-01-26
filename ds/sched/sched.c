/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Ben Shomroni ||| Date 15.12.19  */
/***********************************************************************/
/*     This file contains all the functions of the header sched.h     */
/***********************************************************************/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <time.h> /* time_t */
#include <unistd.h> /* sleep */

#include "sched.h" /* scheduler_t */
#include "pqueue.h" /* p_queue_t */
#include "task.h" /* task_t */
#include "uid.h" /* ilrd_uid_t */

#define CURR_TASK (sched->current_task)
#define PQUEUE (sched->pqueue)

struct scheduler
{
	p_queue_t *pqueue;
	int to_stop;
	task_t *current_task;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static int TimeCompare(const void *new_data, const void *data, void *param);
static int IsUidMatched(const void *new_data, const void *data);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

scheduler_t *SchedCreate()
{
	scheduler_t *sched = NULL;

	sched = (scheduler_t*)malloc(sizeof(scheduler_t));
	if (NULL == sched)
	{
		return NULL;
	}

	sched->pqueue = PQCreate(TimeCompare, NULL);
	if (NULL == sched->pqueue)
	{
		free(sched);

		return NULL;
	}

	sched->current_task = NULL;
	sched->to_stop = 0;

	return sched;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SchedDestroy(scheduler_t *sched)
{
	assert(NULL != sched);

	SchedClear(sched);	
	PQDestroy(sched->pqueue);
	
	free(sched);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
size_t SchedSize(const scheduler_t *sched)
{
	assert(NULL != sched);

	return PQSize(PQUEUE);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int SchedIsEmpty(const scheduler_t *sched)
{
	assert(NULL != sched);

	return PQIsEmpty(PQUEUE);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SchedClear(scheduler_t *sched)
{
	assert(NULL != sched);

	while (!SchedIsEmpty(sched))
	{
		TaskDestroy((task_t*)PQDequeue(PQUEUE));
	}
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ilrd_uid_t *SchedAddTask(scheduler_t *sched, task_action_func action_func,
										void *param,
										size_t interval_in_seconds,
										task_destroy_func destroy_func)
{
    task_t *task = NULL;
	int status = 0;

	assert (NULL != sched);

	task = TaskCreate(action_func, param, interval_in_seconds, destroy_func);
	if(NULL == task)
	{
		return NULL;
	}
   
	status = PQEnqueue(PQUEUE, task);

	return (status) ? NULL : TaskGetUid(task);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SchedRemoveTask(scheduler_t *sched, ilrd_uid_t *uid)
{
	task_t *task = NULL;

	assert(NULL != sched);
	assert(NULL != uid);

	task = (task_t*)PQErase(PQUEUE, IsUidMatched, uid);
	if (NULL != task )
	{
		TaskDestroy(task);
	}
	else if (NULL != CURR_TASK && UIDIsSame(*uid, *TaskGetUid(CURR_TASK)))
	{
		task = CURR_TASK;
		TaskDestroy(task);
		CURR_TASK = NULL;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int SchedRun(scheduler_t *sched)
{
	size_t delay_time = 0;

	assert(NULL != sched);

	sched->to_stop = 0;

	while (!(sched->to_stop) && !SchedIsEmpty(sched))
	{
		delay_time = TaskGetTimeToRun((task_t*)PQPeek(PQUEUE)) - time(NULL);				 
					  
		if (delay_time > 0)
		{
			printf("%ld\n", delay_time); /* It doesn't run without it :( */
			while (0 < delay_time)
			{							
				sleep(1);
				--delay_time;
			}
		}

		CURR_TASK = PQDequeue(PQUEUE);

		delay_time = TaskExecuteAction(CURR_TASK);

		if (delay_time > 0 && (NULL != CURR_TASK))
		{
			TaskSetTimeToRun(CURR_TASK, delay_time);
			PQEnqueue(PQUEUE, CURR_TASK);
		}
		else if (NULL != CURR_TASK)
		{
			TaskDestroy(CURR_TASK);
		}
 	}

    return sched->to_stop;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SchedStop(scheduler_t *sched)
{
	assert(NULL != sched);

	sched->to_stop = 1;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static int IsUidMatched(const void *new_data, const void *data)
{
	return UIDIsSame(*TaskGetUid((task_t *)new_data), *(ilrd_uid_t*)data);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static int TimeCompare(const void *new_data, const void *data, void *param)
{
    size_t new_time = TaskGetTimeToRun((task_t*)new_data);
    size_t old_time = TaskGetTimeToRun((task_t*)data);
	(void)param;

	if (new_time > old_time)
	{
		return 1;
	}
	else if (new_time < old_time)
    {
		return -1;
	}

	return 0;
}


