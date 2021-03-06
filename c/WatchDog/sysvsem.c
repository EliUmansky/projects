/******************************************************************************/
/*                Author: Eli Umansky || Reviewer: Daniel Oren               */
/******************************************************************************/
/*               Exercise: System V Semaphore || Date: 16.02.20               */
/******************************************************************************/

#include <unistd.h> /* sleep */
#include <semaphore.h> /* sem_t */
#include <assert.h> /* assert */
#include <sys/types.h> /* semget */
#include <sys/sem.h> /* semctl */
#include <sys/ipc.h> /* ftok */
#include <errno.h> /* errno */

#include "sysvsem.h" /* semid_t */

#define PROJECT_ID (444)
#define SUCCESS (0)
#define FAILURE (-1)
#define READY (1)
#define NOT_READY (0)
#define MAX_TRIES (10)
#define NOT_LAST_PROCESS (0)
#define CHECK(x) if(FAILURE == x) {return FAILURE;} 

enum operation
{
	WAIT = -1,
	WAIT_FOR_ZERO = 0,
	POST = 1
};

union semun 
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

static semid_t InitSems(semid_t sem_id, size_t num_of_sems, 
						const int *init_values_list);
static semid_t GetSemID(key_t key, size_t num_of_sems);
static int SemOperate(semid_t sem_id, size_t sem_index, int operation, int undo);
/* -------------------------------------------------------------------------- */
semid_t SysVSemCreate(size_t num_of_sems, const int *init_values_list, 
                      const char *path)
{
	key_t key = 0;
	semid_t sem_id = 0;

	assert (NULL != path);
	assert (0 < num_of_sems);

	key = ftok(path, PROJECT_ID);
	CHECK(key);

	sem_id = semget(key, num_of_sems + 2, IPC_CREAT | IPC_EXCL | 0666);
	if (0 <= sem_id)
	{
		return InitSems(sem_id, num_of_sems, init_values_list);
	}
	else if (EEXIST == errno && FAILURE == sem_id)
	{
        return GetSemID(key, num_of_sems);
	}		
	else
	{
		return FAILURE;
	}
}
/* -------------------------------------------------------------------------- */
int SysVSemDestroy(semid_t sem_id)
{
	union semun arg = {0};
    struct semid_ds buf = {0};
	size_t count_sem_index = 0;
	size_t binary_index = 0;	
	size_t num_of_sems = 0;
	int status = SUCCESS;

	arg.buf = &buf;
	status = semctl(sem_id, count_sem_index , IPC_STAT, arg);
	CHECK(status);	
	num_of_sems = arg.buf->sem_nsems;

	count_sem_index = num_of_sems - 2;
	binary_index = num_of_sems - 1;

	status = SysVSemWait(sem_id, binary_index, 1);
	CHECK(status);
	status = SysVSemWait(sem_id, count_sem_index, 1);
	CHECK(status);
	
	if (0 == SysVSemGetVal(sem_id, count_sem_index))
	{
		return semctl(sem_id, 0, IPC_RMID);	
	}

	status = SysVSemPost(sem_id, binary_index, 1);		
	CHECK(status);

	return NOT_LAST_PROCESS;
}
/* -------------------------------------------------------------------------- */
int SysVSemWait(semid_t sem_id, size_t sem_index, int undo)
{
	return SemOperate(sem_id, sem_index, WAIT, undo);
}
/* -------------------------------------------------------------------------- */
int SysVSemWaitForZero(semid_t sem_id, size_t sem_index)
{
	return SemOperate(sem_id, sem_index, WAIT_FOR_ZERO, 1);
}
/* -------------------------------------------------------------------------- */
int SysVSemPost(semid_t sem_id, size_t sem_index, int undo)
{
	return SemOperate(sem_id, sem_index, POST, undo);
}
/* -------------------------------------------------------------------------- */
int SysVSemGetVal(semid_t sem_id, size_t sem_index)
{
	return semctl(sem_id, sem_index, GETVAL);
}
/* -------------------------------------------------------------------------- */
int SysVSemSetVal(semid_t sem_id, size_t sem_index, int value)
{
	return semctl(sem_id, sem_index, SETVAL, value);
}
/* -------------------------------------------------------------------------- */
int SysVSemAbort(semid_t sem_id)
{
	return semctl(sem_id, 0, IPC_RMID);	
}
/* -------------------------------------------------------------------------- */
static semid_t InitSems(semid_t sem_id, size_t num_of_sems, 
						const int *init_values_list)
{
	size_t i = 0;
	int status = SUCCESS;

	if (NULL == init_values_list)
	{
		for (i = 0; i < num_of_sems; ++i)	
		{		
			status = SysVSemSetVal(sem_id, i, 0);
			CHECK(status);
		}			
	}
	else
	{
		for (i = 0; i < num_of_sems; ++i)
		{				
			status = SysVSemSetVal(sem_id, i, init_values_list[i]);
			CHECK(status);	
		}
	}

	status = SysVSemSetVal(sem_id, num_of_sems, 0);
	CHECK(status);
	status = SysVSemSetVal(sem_id, num_of_sems + 1, 1);
	CHECK(status);
	status = SysVSemPost(sem_id, num_of_sems, 1);
	CHECK(status);

	return sem_id;
}
/* -------------------------------------------------------------------------- */
static semid_t GetSemID(key_t key, size_t num_of_sems)
{
	union semun arg = {0};
    struct semid_ds buf = {0};
	size_t i = 0;
	int status = SUCCESS;
	int is_ready = NOT_READY;
	semid_t sem_id = 0;

	arg.buf = &buf;

	sem_id = semget(key, num_of_sems + 2, 0);
    CHECK(sem_id);

    for (i = 0; i < MAX_TRIES && NOT_READY == is_ready; ++i)
	{
        status = semctl(sem_id, num_of_sems + 2, IPC_STAT, arg);
		CHECK(status);
		if (0 != arg.buf->sem_otime)
		{
            is_ready = READY;
        }
		else 
		{
            sleep(1);
        }
    }

    if (NOT_READY == is_ready)
	{
        errno = ETIME;
        return FAILURE;
    }

	status = SysVSemWait(sem_id, num_of_sems + 1, 1);
	CHECK(status);	
	status = SysVSemPost(sem_id, num_of_sems, 1);
	CHECK(status);
	status = SysVSemPost(sem_id, num_of_sems + 1, 1);
	CHECK(status);

	return sem_id;
}
/* -------------------------------------------------------------------------- */
static int SemOperate(semid_t sem_id, size_t sem_index, int operation, int undo)
{
	struct sembuf buf = {0};

	buf.sem_num = sem_index;
	buf.sem_op = operation;
	buf.sem_flg = (undo) ? SEM_UNDO : 0;
	
	return semop(sem_id, &buf, 1);
}
