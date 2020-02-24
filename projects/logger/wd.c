#define _POSIX_C_SOURCE 200112L
#include <stdio.h> /* fopen */
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
#define WD_PATH "/home/student/git/projects/logger/wd_exec.out"

enum sems {APP, IS_CREATED_FLAG};
enum priority {LOW, HIGH, CRITICAL, FATAL};

FILE *logger;
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
static char *AppOrWD();
static void LogInt(char *message, int priority, char *section, int param);
static void Log(char *message, int priority, char *section);

int WDStart(char *argv[])
{
	struct sigaction sa = {0};
	pthread_t thread = 0;
	int status = SUCCESS;
	char path[MAX_PATH_SIZE];
	
	Log("Log file opened", LOW, "setup");
	
	status = SetPath(argv, path);
	if (FAILURE == status)
	{
		Log("Failed to set path", FATAL, "setup");		
	}

	sem_id = SysVSemCreate(NUM_OF_SEMS, NULL, path);
	if (FAILURE == status)
	{
		Log("Failed to create semaphores", FATAL, "setup");		
		return FAILURE;	
	}
	LogInt("sem_id is received, sem_id ", HIGH, "setup", sem_id);
	
	status = InitSigaction(sa);
	if (FAILURE == status)
	{
		Log("Failed to set up handlers", FATAL, "setup");	
		return FAILURE;	
	}
	
	
	if (0 == SysVSemGetVal(sem_id, IS_CREATED_FLAG))
	{  
		status = setenv("WATCHDOG_MASTER_PATH", path, 0);
		CHECK_IF_FAILED(status);
		Log("Environmental variable set", LOW, "setup");
		status = SysVSemPost(sem_id, IS_CREATED_FLAG, 0);
		if (FAILURE == status)
		{
			Log("Failed to alter semaphore", FATAL, "setup");		
			return FAILURE;
		}
		Log("Set Created flag", HIGH, "setup");	
				
		status = SetupWD(argv);
		if (FAILURE == status)
		{
			Log("Failed to create WD", FATAL, "setup");		
			return FAILURE;
		}
		Log("Watchdog created", HIGH, "setup");			
	}
	else
	{
		target = getppid();
	}

	status = SetScheduler(argv);
	if (FAILURE == status)
	{
		Log("Failed to create scheduler", FATAL, "setup");	
		return FAILURE;	
	}

	status = SysVSemPost(sem_id, APP, 0);
	if (FAILURE == status)
	{
		Log("Failed to alter semaphore", FATAL, "setup");		
		return FAILURE;
	}

	if (is_watchdog)
	{
		Log("Scheduler is about to run", HIGH, "run");			
		SchedRun(sched);
		SchedDestroy(sched);
		Log("Scheduler Destroyed", LOW, "run");			
	}
	else
	{
		status = pthread_create(&thread, NULL, &SchedStart, NULL);
		if (FAILURE == status)
		{
			Log("Failed to create thread", FATAL, "setup");		
			return FAILURE;
		}
		Log("Thread Created", LOW, "setup");			
	}

	return SUCCESS;
}

int WDEnd(size_t timeout)
{
	do 
	{
		kill(target, SIGUSR2);
		LogInt("Signal2 sent to target ", LOW, "run", target);			
		--timeout;
		sleep(1);
	}
	while (timeout && 2 != is_watchdog_dead);

	if (2 == is_watchdog_dead)
	{
		SysVSemAbort(sem_id);
		Log("Semaphores Destroyed", LOW, "setup");			

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
	Log("Signal handler 1 set", LOW, "setup");

	sigfillset(&sa.sa_mask);
	sa.sa_sigaction = HandleDeath;
	status = sigaction(SIGUSR2, &sa, NULL);   
 	CHECK_IF_FAILED(status);
	Log("Signal handler 2 set", LOW, "setup");

	return status;
}

static int SetupWD(char **argv)
{
	target = fork();
	if (FAILURE == target)
	{
		Log("Failed to create a child process", FATAL, "setup");	
	}

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
	if (FAILURE != status)
	{
		Log("Scheduler created", HIGH, "setup");			
	}
	else
	{
		return FAILURE;
	}
	task_id = SchedAddTask(sched, &SendLifeSignal, NULL, 1, DestroyFunc);
	if (FAILURE == status)
	{
		Log("Failed to add task", FATAL, "setup");		
		return FAILURE;		
	}
	else
	{
		Log("Task1 added", LOW, "setup");
	}
	task_id = SchedAddTask(sched, &CheckLifeAndRevive, argv, 5, DestroyFunc);
	if (FAILURE == status)
	{
		Log("Failed to add task", FATAL, "setup");		
		return FAILURE;
	}
	else
	{
		Log("Task2 added", LOW, "setup");
	}

	return SUCCESS;
}

static size_t SendLifeSignal(void *param)
{
	(void)param;
	
	kill(target, SIGUSR1);
	LogInt("Signal1 sent to target ", LOW, "run", target);			

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
		Log("Positive life count, set to 0", LOW, "run");	
		life_count = 0;
	}
	else
	{
		Log("Revive", CRITICAL, "run");
		Revive(param);
	}

	return 10;
}

static void DestroyFunc(void *param)
{
	(void)param;
}

static void ReceiveLifeSignal(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)context;

	if (SIG_PID == target)
	{	
		++life_count;
		LogInt("life count", LOW, "run", life_count);
	}	
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
			kill(target, SIGUSR2);
			LogInt("Signal2 sent to target ", HIGH, "run", target);
		}
	}	
}

static void *SchedStart(void *param)
{
	(void)param;

	pthread_detach(pthread_self());

	Log("Scheduler is about to run", HIGH, "run");			
	SchedRun(sched);
	SchedDestroy(sched);
	Log("Scheduler Destroyed", LOW, "run");			

	is_watchdog_dead = 2;

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
		if (!is_watchdog)
		{
			strcpy(path, WD_PATH);
		}

		execvp(path, argv);
	}

	SysVSemWait(sem_id, APP, 0);
	Log("Post semaphore", HIGH, "run");			
}

static void Log(char *message, int priority, char *section)
{
	time_t clock = time(NULL);

	logger = fopen("logger.txt", "a");
	flockfile(logger);
	fprintf(logger, "%s [%s] [pid %d] [%s section], priority = %d, %s\n", 
	ctime(&clock), AppOrWD(), getpid(), section , priority, message);
	funlockfile(logger);	
	fclose(logger);
}

static void LogInt(char *message, int priority, char *section, int param)
{
	time_t clock = time(NULL);

	logger = fopen("logger.txt", "a");
	flockfile(logger);
	fprintf(logger, "%s [%s] [pid %d] [%s section], priority = %d, %s = %d\n", 
	ctime(&clock), AppOrWD(), getpid(), section, priority, message, param);
	funlockfile(logger);	
	fclose(logger);
}

static char *AppOrWD()
{
	return (is_watchdog) ? "WD" : "APP";
}
