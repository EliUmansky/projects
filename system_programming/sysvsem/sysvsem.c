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

union semun 
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

static semid_t InitSems(semid_t sem_id, size_t num_of_sems, 
						const int *init_values_list);
static semid_t GetSemID(key_t key, size_t num_of_sems);
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
	if (EEXIST == errno && FAILURE == sem_id)
	{
        return GetSemID(key, num_of_sems);
	}
	else if (FAILURE == sem_id)
	{
		return FAILURE;
	}		
	else
	{
		return InitSems(sem_id, num_of_sems, init_values_list);
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

	status = SysVSemWait(sem_id, binary_index);
	CHECK(status);
	status = SysVSemWait(sem_id, count_sem_index);
	CHECK(status);
	
	if (0 == semctl(sem_id, count_sem_index, GETVAL, arg))
	{
		return semctl(sem_id, 0, IPC_RMID);	
	}

	status = SysVSemPost(sem_id, binary_index);		
	CHECK(status);

	return NOT_LAST_PROCESS;
}
/* -------------------------------------------------------------------------- */
int SysVSemWait(semid_t sem_id, size_t sem_index)
{
	struct sembuf buf = {0};	

	buf.sem_num = sem_index;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;

	return semop(sem_id, &buf, 1);
}
/* -------------------------------------------------------------------------- */
int SysVSemWaitForZero(semid_t sem_id, size_t sem_index)
{
	struct sembuf buf = {0};	

	buf.sem_num = sem_index;
	buf.sem_op = 0;
	buf.sem_flg = SEM_UNDO;

	return semop(sem_id, &buf, 1);
}
/* -------------------------------------------------------------------------- */
int SysVSemPost(semid_t sem_id, size_t sem_index)
{
	struct sembuf buf = {0};	

	buf.sem_num = sem_index;
	buf.sem_op = 1;
	buf.sem_flg = SEM_UNDO;

	return semop(sem_id, &buf, 1);
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
			status = semctl(sem_id, i, SETVAL, 0);
			CHECK(status);
		}			
	}
	else
	{
		for (i = 0; i < num_of_sems; ++i)
		{				
			status = semctl(sem_id, i, SETVAL, init_values_list[i]);
			CHECK(status);	
		}
	}

	status = semctl(sem_id, num_of_sems, SETVAL, 0);
	CHECK(status);
	status = semctl(sem_id, num_of_sems + 1, SETVAL, 1);
	CHECK(status);
	status = SysVSemPost(sem_id, num_of_sems);
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

	status = SysVSemWait(sem_id, num_of_sems + 1);
	CHECK(status);	
	status = SysVSemPost(sem_id, num_of_sems);
	CHECK(status);
	status = SysVSemPost(sem_id, num_of_sems + 1);
	CHECK(status);

	return sem_id;
}
