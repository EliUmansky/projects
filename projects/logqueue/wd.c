#define _POSIX_C_SOURCE 200112L
#include <stdio.h> /* fopen */
#include <unistd.h> /* sleep */
#include <signal.h> /* kill */
#include <pthread.h> /* pthread_create */
#include <sys/types.h> /* waitpid */
#include <sys/wait.h> /* waitpid */
#include <stdlib.h> /* setenv */
#include <string.h> /* strcpy */
#include <semaphore.h> /* sem_t */

#include "wd.h"
#include "sysvsem.h" /* SysVSemCreate */
#include "sched.h" /* scheduler_t */
#include "uid.h" /* IsUidBad */
#include "queue.h" /* queue_t */

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
#define NO_PARAM (-1)

enum sems {APP, IS_CREATED_FLAG};
enum priority {LOW, HIGH, CRITICAL, FATAL};

typedef struct
{
	int time;
	int param;
} data_t;
 
FILE *logger;
int is_watchdog;
static int life_count;
static pid_t target;
static pid_t thread_queue;
static int is_watchdog_dead;
static int sem_id;
static scheduler_t *sched;
static int is_log_run;
static queue_t *message_queue;
static queue_t *time_queue;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t log_sem;

static const char *log_open = "[LOW][setup] opened log" ;
static const char *sem_id_receive = "[HIGH][setup] sem_id received, sim_id = ";
static const char *set_sig1_hand = "[LOW][setup] signal handler1 set ";
static const char *set_sig2_hand = "[LOW][setup] signal handler2 set ";
static const char *set_env_var = "[LOW][setup] environmental variable set ";
static const char *set_par_flag = "[HIGH][setup] parent flag set ";
static const char *wd_create = "[HIGH][setup] watchdog created ";
static const char *targ_id = "[LOW][setup] target_id = ";
static const char *post_sem = "[HIGH][setup] post semaphore ";
static const char *sched_create = "[HIGH][setup] scheduler created ";
static const char *task1_create = "[LOW][setup] task1 created ";
static const char *task2_create = "[LOW][setup] task2 created ";
static const char *thread_create = "[LOW][setup] thread created ";
static const char *sched_run = "[HIGH][setup] scheduler starts running ";

static const char *sig1_sent = "[LOW][run] signal1 sent to target = ";
static const char *sig1_receive = "[LOW][run] signal1 received ";
static const char *lifecount = "[LOW][run] life count = ";
static const char *set_lifecount = "[LOW][run] life count is positive, set to 0 ";
static const char *revive = "[CRITICAL][run] revive ";
static const char *sig2_sent = "[HIGH][cleanup] signal2 sent to target = ";
static const char *sig2_receive = "[LOW][run] signal2 received ";
static const char *destroy_sched = "[LOW][cleanup] destroying scheduler ";
static const char *destroy_sems = "[LOW][cleanup] destroying semaphores ";

static const char *fail_create_sems = "[FATAL][err] failed to create semaphores ";
static const char *fail_set_hand = "[FATAL][err] failed to set up handlers ";
static const char *fail_alter_sem = "[FATAL][err] failed to alter semaphore ";
static const char *fail_create_wd = "[FATAL][err] failed to create watchdog ";
static const char *fail_create_sched = "[FATAL][err] failed to create scheduler ";
static const char *fail_add_task = "[FATAL][err] failed to add task ";
static const char *fail_create_thread = "[FATAL][err] failed to create thread ";
static const char *fail_create_process = "[FATAL][err] Failed to create a child process ";
static const char *fail_set_path = "[FATAL][err] failed to set path ";
static const char *fail_set_env_var = "[FATAL][err] failed to set environmental variable ";
static const char *fail_destroy_sems = "[FATAL][err] failed to destroy semaphores ";

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
static void Log(const char *message, int param);
static void *RunQueue(void *param);
static void EndLog();
static void StartLog();

int WDStart(char *argv[])
{
	struct sigaction sa = {0};
	pthread_t thread = 0;
	int status = SUCCESS;
	char path[MAX_PATH_SIZE];
	
	StartLog();
	Log(log_open, NO_PARAM);
	
	status = SetPath(argv, path);
	if (FAILURE == status)
	{
		Log(fail_set_path, NO_PARAM);		
	}

	sem_id = SysVSemCreate(NUM_OF_SEMS, NULL, path);
	if (FAILURE == status)
	{
		Log(fail_create_sems, NO_PARAM);		
		return FAILURE;	
	}
	Log(sem_id_receive, sem_id);
	
	status = InitSigaction(sa);
	if (FAILURE == status)
	{
		Log(fail_set_hand, NO_PARAM);	
		return FAILURE;	
	}
	
	if (0 == SysVSemGetVal(sem_id, IS_CREATED_FLAG))
	{  
		status = setenv("WATCHDOG_MASTER_PATH", path, 0);
		if (FAILURE == status)
		{
			Log(fail_set_env_var, NO_PARAM);
		}
		Log(set_env_var, NO_PARAM);

		status = SysVSemPost(sem_id, IS_CREATED_FLAG, 0);
		if (FAILURE == status)
		{
			Log(fail_alter_sem, NO_PARAM);		
			return FAILURE;
		}
		Log(set_par_flag, NO_PARAM);		
				
		status = SetupWD(argv);
		if (FAILURE == status)
		{
			Log(fail_create_wd, NO_PARAM);			
			return FAILURE;
		}
		Log(wd_create, NO_PARAM);			
	}
	else
	{
		target = getppid();
	}

	status = SetScheduler(argv);
	if (FAILURE == status)
	{
		Log(fail_create_sched, NO_PARAM);			
		return FAILURE;	
	}
	Log(sched_create, NO_PARAM);			

	status = SysVSemPost(sem_id, APP, 0);
	if (FAILURE == status)
	{
		Log(fail_alter_sem, NO_PARAM);		
		return FAILURE;
	}
	Log(post_sem, NO_PARAM);		

	if (is_watchdog)
	{
		Log(sched_run, NO_PARAM);		
		SchedRun(sched);
		SchedDestroy(sched);
		Log(destroy_sched, NO_PARAM);		
	}
	else
	{
		status = pthread_create(&thread, NULL, &SchedStart, NULL);
		if (FAILURE == status)
		{
			Log(fail_create_thread, NO_PARAM);		
			return FAILURE;
		}
		Log(thread_create, NO_PARAM);		
	}

	return SUCCESS;
}

int WDEnd(size_t timeout)
{
	do 
	{
		kill(target, SIGUSR2);
		Log(sig2_sent, target);		
		--timeout;
		sleep(1);
	}
	while (timeout && 2 != is_watchdog_dead);

	if (2 == is_watchdog_dead)
	{
		if (FAILURE == SysVSemAbort(sem_id))
		{
			Log(fail_destroy_sems, NO_PARAM);			
		}
		Log(destroy_sems, NO_PARAM);			

		return SUCCESS;
	}

	EndLog();

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
	if (FAILURE == status)
	{
		Log(fail_set_hand, NO_PARAM);
	}
	Log(set_sig2_hand, NO_PARAM);

	sigfillset(&sa.sa_mask);
	sa.sa_sigaction = HandleDeath;
	status = sigaction(SIGUSR2, &sa, NULL);   
	if (FAILURE == status)
	{
		Log(fail_set_hand, NO_PARAM);
	}
	Log(set_sig1_hand, NO_PARAM);

	return status;
}

static int SetupWD(char **argv)
{
	target = fork();
	if (FAILURE == target)
	{
		Log(fail_create_process, NO_PARAM);	
		return FAILURE;
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
	if (NULL == sched)
	{
		return FAILURE;
	}

	Log(sched_create, NO_PARAM);			

	task_id = SchedAddTask(sched, &SendLifeSignal, NULL, 1, DestroyFunc);
	if (NULL == task_id)
	{
		Log(fail_add_task, NO_PARAM);	
		return FAILURE;		
	}

	Log(task1_create, NO_PARAM);	

	task_id = SchedAddTask(sched, &CheckLifeAndRevive, argv, 5, DestroyFunc);
	if (NULL == task_id)
	{
		Log(fail_add_task, NO_PARAM);	
		return FAILURE;		
	}

	Log(task2_create, NO_PARAM);	

	return SUCCESS;
}

static size_t SendLifeSignal(void *param)
{
	(void)param;
	
	kill(target, SIGUSR1);
	Log(sig1_sent, target);	

	if (is_watchdog_dead)
	{
		SchedStop(sched);
	}

	return 2;
}

static size_t CheckLifeAndRevive(void *param)
{
	Log(sig2_receive, target);	

	if (0 < life_count)
	{
		Log(set_lifecount, NO_PARAM);	
		life_count = 0;
	}
	else
	{
		Log(revive, NO_PARAM);	
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
		Log(sig1_receive, NO_PARAM);	
		Log(lifecount, life_count);	
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
			Log(sig2_sent, target);	
		}
	}	
}

static void *SchedStart(void *param)
{
	(void)param;

	pthread_detach(pthread_self());

	Log(sched_run, NO_PARAM);	
	SchedRun(sched);
	SchedDestroy(sched);
	Log(destroy_sched, NO_PARAM);	

	is_watchdog_dead = 2;

	return NULL;
}

static void Revive(char **argv)
{
	char path[MAX_PATH_SIZE];

	strcpy(path, getenv("WATCHDOG_MASTER_PATH"));

	waitpid(target, NULL, 0);

	target = fork();
	if (0 > target)
	{
		Log(fail_create_process, NO_PARAM);	
	}
	
	if (0 == target)
	{	
		if (!is_watchdog)
		{
			strcpy(path, WD_PATH);
		}

		execvp(path, argv);
	}
	
	if (FAILURE == SysVSemWait(sem_id, APP, 0))
	{
		Log(fail_alter_sem, NO_PARAM);			
	}
}

static void Log(const char *message, int param)
{
	data_t data = {0};

	data.time = time(NULL);
	data.param = param;

	pthread_mutex_lock(&mutex);
	QueEnqueue(message_queue, message);
	QueEnqueue(time_queue, *(void**)&data);
	pthread_mutex_unlock(&mutex);

	sem_post(&log_sem);
}

static char *AppOrWD()
{
	return (is_watchdog) ? "[WD]" : "[APP]";
}

static void StartLog()
{
	pthread_t thread_queue = 0;

	message_queue = QueCreate();
	time_queue = QueCreate();
	sem_init(&log_sem, 1, 0);

	is_log_run = 1;
	pthread_create(&thread_queue, NULL, &RunQueue, NULL);
}

static void EndLog()
{
	is_log_run = 0;
	pthread_join(thread_queue, NULL);
}

static void *RunQueue(void *param)
{
	char buf[100] = {0};
	char help_buf[20] = {0};

	(void)param;

	while (is_log_run || !QueIsEmpty(time_queue))
	{
		if (!QueIsEmpty(time_queue))
		{
			sprintf(help_buf, "%d", (*(data_t*)QuePeek(time_queue)).time);
			strcpy(buf, help_buf);
			strcpy(buf, AppOrWD());
			strcpy(buf, QuePeek(message_queue));
			QueDequeue(message_queue);
		
			if (0 <= (*(data_t*)QuePeek(time_queue)).param)
			{			
				sprintf(help_buf, "%d", (*(data_t*)QuePeek(time_queue)).param);
				strcpy(buf, help_buf);
			}			
			QueDequeue(time_queue);

			flockfile(logger);
			fprintf(logger, buf);
			funlockfile(logger);
		}
	}
	
	sem_wait(&log_sem);

	return NULL;
}
	
