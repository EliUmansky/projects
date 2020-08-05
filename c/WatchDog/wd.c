#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <unistd.h> /* sleep */
#include <signal.h> /* kill */
#include <pthread.h> /* pthread_create */
#include <sys/types.h> /* waitpid */
#include <sys/wait.h> /* waitpid */
#include <stdlib.h> /* setenv */
#include <string.h> /* strcpy */

#include "wd.h"
#include "sysvsem.h" /* SysVSemCreate */
#include "sched.h" /* scheduler_t */
#include "uid.h" /* IsUidBad */

#define SUCCESS (0)
#define FAILURE (-1)
#define NUM_OF_SEMS (2)
#define SIG_PID (info->si_pid)
#define CHECK_IF_FAILED(x) if(FAILURE == x) {return FAILURE;} 
#define CHECK_IF_PTR_FAILED(x) if(NULL == x) {return FAILURE;} 
#define CHECK_UID(x) if(UIDIsBad(*x)) {return FAILURE;}
#define CHECK_THREAD(x) if(0 != x) {return FAILURE;}
#define MAX_PATH_SIZE (80)
#define WD_PATH "/home/student/git/wd/wd_exec.out"

enum sems {APP, IS_CREATED_FLAG};

int is_watchdog;
static int life_count;
static pid_t target;
static int is_watchdog_dead;
static int sem_id;
static scheduler_t *sched;

static int SetPath(char **argv, char *path);
static char *GetAppName(char *arg);
static int InitSigaction(struct sigaction sa);
static int SetupWD(char **argv);
static size_t SendLifeSignal(void *param);
static void DestroyFunc(void *param);
static void ReceiveLifeSignal(int sig, siginfo_t *info, void *context);
static void HandleDeath(int sig, siginfo_t *info, void *context);
static void *SchedStart(void *param);
static void Revive(char **argv);
static size_t CheckLifeAndRevive(void *param);
static int SetScheduler(char **agrv);

int WDStart(char *argv[])
{
	struct sigaction sa = {0};
	pthread_t thread = 0;
	int status = SUCCESS;
	char path[MAX_PATH_SIZE];
	
	status = SetPath(argv, path);
	CHECK_IF_FAILED(status);
	sem_id = SysVSemCreate(NUM_OF_SEMS, NULL, path);
	CHECK_IF_FAILED(sem_id);
	fprintf(stderr, "path is %s\n", path);
	status = InitSigaction(sa);
	CHECK_IF_FAILED(status);
	
	if (0 == SysVSemGetVal(sem_id, IS_CREATED_FLAG))
	{  
		status = setenv("WATCHDOG_MASTER_PATH", path, 0);
		CHECK_IF_FAILED(status);
		status = SysVSemPost(sem_id, IS_CREATED_FLAG, 0);	
		CHECK_IF_FAILED(status);
		status = SetupWD(argv);
		CHECK_IF_FAILED(status);
	}
	else
	{
		target = getppid();
	}

	status = SetScheduler(argv);
	CHECK_IF_FAILED(status);	

	status = SysVSemPost(sem_id, APP, 0);
	CHECK_IF_FAILED(status);

	if (is_watchdog)
	{
		SchedRun(sched);
		SchedDestroy(sched);
	}
	else
	{
		status = pthread_create(&thread, NULL, &SchedStart, NULL);
		CHECK_THREAD(status);
	}

	return SUCCESS;
}

int WDEnd(size_t timeout)
{
	do 
	{
		kill(target, SIGUSR2);
		--timeout;
		sleep(1);
	}
	while (timeout && 2 != is_watchdog_dead);

	if (2 == is_watchdog_dead)
	{
		SysVSemAbort(sem_id);
		fprintf(stderr, "abort\n");	
		return SUCCESS;
	}

	return FAILURE;
}

static int SetPath(char **argv, char *path)
{
	char *status_ptr = NULL;

	if (!is_watchdog)
	{
		status_ptr = getcwd(path, MAX_PATH_SIZE);
		CHECK_IF_PTR_FAILED(status_ptr);
		strcat(path, "/");
		strcat(path, GetAppName(argv[0]));
	}
	else
	{
		strcpy(path, getenv("WATCHDOG_MASTER_PATH"));
	}

	return SUCCESS;
}

static char *GetAppName(char *arg)
{
	char *runner = arg;

	while ('\0' != *runner)
	{
		++runner;
	}
	while (arg < runner && '/' != *(runner - 1))
	{
		--runner;
	}

	return runner;
}

static int InitSigaction(struct sigaction sa)
{
	int status = SUCCESS;
	
	sa.sa_flags = SA_SIGINFO;

	sa.sa_sigaction = ReceiveLifeSignal;
	status = sigaction(SIGUSR1, &sa, NULL); 
	CHECK_IF_FAILED(status);

	sigfillset(&sa.sa_mask);
	sa.sa_sigaction = HandleDeath;
	status = sigaction(SIGUSR2, &sa, NULL);   
 	CHECK_IF_FAILED(status);

	return status;
}

static int SetupWD(char **argv)
{
	target = fork();
	if (0 == target)
	{
		execvp(WD_PATH, argv);
	}

	return SysVSemWait(sem_id, APP, 0);
}

static int SetScheduler(char **argv)
{
	ilrd_uid_t *task_id = NULL;

	sched = SchedCreate();
	CHECK_IF_PTR_FAILED(sched);
	task_id = SchedAddTask(sched, &SendLifeSignal, NULL, 1, DestroyFunc);
	CHECK_UID(task_id);
	task_id = SchedAddTask(sched, &CheckLifeAndRevive, argv, 5, DestroyFunc);
	CHECK_UID(task_id);

	return SUCCESS;
}

static size_t SendLifeSignal(void *param)
{
	(void)param;
	
	kill(target, SIGUSR1);

	if (is_watchdog_dead)
	{
		SchedStop(sched);
	}

	return 2;
}

static size_t CheckLifeAndRevive(void *param)
{
	(void)param;

	if (0 < life_count)
	{
		life_count = 0;
	}
	else
	{
		fprintf(stderr, "Let's revive the MF\n");
		Revive(param);
	}

	return 10;
}

static void DestroyFunc(void *param)
{
	(void)param;
	fprintf(stderr, "destroy\n");
}

static void ReceiveLifeSignal(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)context;

	if (SIG_PID == target)
	{	
		++life_count;
	}	
	fprintf(stderr, "life count = %d, is_watchdog = %d\n", life_count, is_watchdog);
}

static void HandleDeath(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)context;

	if (SIG_PID == target)
	{
		is_watchdog_dead = (2 == is_watchdog_dead) ? is_watchdog_dead : 1;

		if (is_watchdog)
		{
			fprintf(stderr, "wd handler\n");		
			kill(target, SIGUSR2);
		}
		else
		{
			fprintf(stderr, "app handler\n");		
		}
	}	
}

static void *SchedStart(void *param)
{
	(void)param;

	pthread_detach(pthread_self());
	SchedRun(sched);
	SchedDestroy(sched);
	is_watchdog_dead = 2;
	fprintf(stderr, "About to die\n");

	return NULL;
}

static void Revive(char **argv)
{
	char path[MAX_PATH_SIZE];

	strcpy(path, getenv("WATCHDOG_MASTER_PATH"));

	waitpid(target, NULL, 0);

	target = fork();
	if (0 == target)
	{	
		fprintf(stderr, "is_watchdog = %d\n", is_watchdog);
		
		if (!is_watchdog)
		{
			strcpy(path, WD_PATH);
		}
		fprintf(stderr, "Let's run %s\n", path);
		execvp(path, argv);
	}

	SysVSemWait(sem_id, APP, 0);
}



