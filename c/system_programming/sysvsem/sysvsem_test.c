
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <semaphore.h> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#include "sysvsem.h"

#define GREEN fprintf(stderr, "\x1b[32m");
#define RED fprintf(stderr, "\x1b[35m");
#define WHITE fprintf(stderr, "\x1b[0m");

static void p1();
static void p2();
static void p3();
static void TestInit();
static void TestRobin();

union semun {
	int val;
	struct semid_ds *buf;   
	unsigned short *array; 
} arg;


int main()
{
	TestInit();
	sleep(1);
	TestRobin();
	
	return 0;
}

static void TestRobin()
{
	pid_t pid1 = 0;
	pid_t pid2 = 0;

	pid1 = fork();
	if (pid1)
	{
		pid2 = fork();
		if (pid2)
		{
			p1();
		}
		else
		{
			p2();
		}
	}
	else
	{
		p3();
	}

}
/* --- Static Functions ---------------------------------------------------- */
static void p1()
{
	int init_val[3] = {1, 0, 0};
	const char *path = "./sysvsem.h";
	semid_t sem_id = 0;
	int count = 3;

	sem_id = SysVSemCreate(3, init_val, path);

	while (count)
	{
		SysVSemWait(sem_id, 0);
		sleep(1);
		fprintf(stderr, "I'm process 1!\n");
		--count;
		SysVSemPost(sem_id, 1);
	}
	sleep(2);
	SysVSemDestroy(sem_id);
}
	
static void p2()
{
	int init_val[3] = {1, 0, 0};
	const char *path = "./sysvsem.h";
	semid_t sem_id = 0;
	int count = 3;

	sem_id = SysVSemCreate(3, init_val, path);

	while (count)
	{
		SysVSemWait(sem_id, 1);
		sleep(1);
		fprintf(stderr, "I'm process 2!\n");
		--count;
		SysVSemPost(sem_id, 2);
	}
	sleep(2);
	SysVSemDestroy(sem_id);
}

static void p3()
{
	int init_val[3] = {1, 0, 0};
	const char *path = "./sysvsem.h";
	semid_t sem_id = 0;
	int count = 3;

	sem_id = SysVSemCreate(3, init_val, path);

	while (count)
	{
		SysVSemWait(sem_id, 2);
		sleep(1);
		fprintf(stderr, "I'm process 3!\n");
		--count;
		SysVSemPost(sem_id, 0);
	}
	sleep(2);
	SysVSemDestroy(sem_id);
}



static void TestInit()
{
	int init_val[5] = {1, 0, 0, 1, 1};
	const char *path = "./sysvsem.h";
	semid_t sem_id = 0;
	union semun arg = {0};
	struct semid_ds semid_ds;
	int i = 0, val = 0;

	sem_id = SysVSemCreate(3, init_val, path);

	arg.buf = &semid_ds;
	semctl(sem_id, 0, IPC_STAT, arg);
	for (i = 0; i < 5; ++i)
	{
		val = semctl(sem_id, i, GETVAL, arg);
		if(val != init_val[i])
		{
			RED
			printf("not good! expected: %d, rec : %d\n", init_val[i], val);
		}
		else
		{
			GREEN
			printf("good! expected: %d\n", init_val[i]);
		}
	}
	sleep(1);			
	
	SysVSemDestroy(sem_id);
}

