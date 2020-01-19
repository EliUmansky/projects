#include <stdio.h> /* size_t */
#include <string.h> /* strcmp */
#include <time.h> /* clock_t */
#include <unistd.h> /* sleep */
#include "dlist.h"
#include "sortlist.h"
#include "pqueue.h"
#include "task.h"
#include "uid.h"

static void test(int expected, int actual, const char* testName);
static int action_func_1(void *param);
static void destroy_func_1(void *param);
static int action_func_2(void *param);
static void destroy_func_2(void *param);
static int action_func_3(void *param);
static void destroy_func_3(void *param);
static int action_func_4(void *param);
static void destroy_func_4(void *param);
void CreateStopFile();
/*static int CheckRTStop(void *sched);
*/void TestFlow1();
void TestFlow2();
void TestFlow3();
void TestFlow4();
void TestFlow5();
void TestFlow6();
void TestFlow7();

struct scheduler
{
	p_queue_t *pqueue;
	int to_stop;
	task_t *current_task;
};

struct task
{
	ilrd_uid_t task_id;
	task_action_func action_func;
	void *param;
	size_t time_to_run;
	task_destroy_func destroy_func;
};

ilrd_uid_t *uid_to_remove;

int main()
{
	TestFlow1();
    TestFlow2();
 	TestFlow3();
	TestFlow4();
	TestFlow5();
	TestFlow6();
	TestFlow7();
	
	return 0;
}

void TestFlow1()
{
	task_t *task = NULL;
	int a = 2;

	puts("\nTask test - Create, Destroy, Check if parameters are correct\n");

	task = TaskCreate(action_func_1, &a, 7, destroy_func_1);

	test(7, (int)(task->time_to_run - time(NULL)), "|| Correct Time to run");
	test(2, *(int*)(task->param), "|| Correct param");

	TaskDestroy(task);
}

void TestFlow2()
{
	task_t *task = NULL;
	int a = 2;

	puts("\nTask test - Action func, Set time, GetUid\n");

	task = TaskCreate(action_func_1, &a, 7, destroy_func_1);

	test(10, TaskExecuteAction(task), "|| Action func returns 10");
	test(0, UIDIsSame(*TaskGetUid(task), g_bad_uid), "|| Is Bad Uid- 0");

	TaskSetTimeToRun(task, 12);
	test(12, (int)(task->time_to_run - time(NULL)), "|| Set time to 12");
	TaskDestroy(task);
}

void TestFlow3()
{
    scheduler_t *sched = NULL;

	puts("\nSched test - Create, Destroy (vlg)");

    sched = SchedCreate();

    SchedDestroy(sched);
}

void TestFlow4()
{
    scheduler_t *sched = NULL;
	int a = 2;

	puts("\nSched test - Add, Size, IsEmpty, Clear\n");

    sched = SchedCreate();

    SchedAddTask(sched, action_func_1, &a, 7, destroy_func_1);
    SchedAddTask(sched, action_func_2, sched, 9, destroy_func_2);
    SchedAddTask(sched, action_func_3, &a, 2, destroy_func_3);
    SchedAddTask(sched, action_func_4, &a, 5, destroy_func_4);

    test(4, SchedSize(sched), "|| Size- 4");
    test(0, SchedIsEmpty(sched), "|| IsEmpty- 0");

	SchedClear(sched);
	test(0, SchedSize(sched), "|| Size after Clear- 0");
    test(1, SchedIsEmpty(sched), "|| IsEmpty after Clear- 1");

    SchedDestroy(sched);
}

void TestFlow5()
{
    scheduler_t *sched = NULL;
	int a = 2;
	ilrd_uid_t *uid1 = NULL;
	ilrd_uid_t *uid2 = NULL;

	puts("\nSched test - Remove\n");

    sched = SchedCreate();

    SchedAddTask(sched, action_func_1, &a, 7, destroy_func_1);
    SchedAddTask(sched, action_func_2, &a, 9, destroy_func_2);
    uid1 = SchedAddTask(sched, action_func_3, &a, 2, destroy_func_3);
    uid2 = SchedAddTask(sched, action_func_4, &a, 5, destroy_func_4);

	SchedRemoveTask(sched, uid1);
	test(3, SchedSize(sched), "|| Size after Removing existing element- 3");
 	SchedRemoveTask(sched, uid2);
	test(2, SchedSize(sched), "|| Size after Removing existing element- 3");

	SchedDestroy(sched);
}

void TestFlow6()
{
    scheduler_t *sched = NULL;
	int a = 2;

	puts("\nSched test - Run (with stop)\n");

    sched = SchedCreate();

    SchedAddTask(sched, action_func_1, &a, 7, destroy_func_1);
    SchedAddTask(sched, action_func_2, sched, 9, destroy_func_2);
    SchedAddTask(sched, action_func_4, &a, 2, destroy_func_3);
    SchedAddTask(sched, action_func_4, &a, 5, destroy_func_4);

	SchedRun(sched);
	SchedDestroy(sched);
}

void TestFlow7()
{
    scheduler_t *sched = NULL;
	
	int a = 2;

	puts("\nSched test - Remove\n");

    sched = SchedCreate();

    SchedAddTask(sched, action_func_1, &a, 7, destroy_func_1);
    SchedAddTask(sched, action_func_1, &a, 9, destroy_func_2);
    uid_to_remove = SchedAddTask(sched, action_func_3, sched, 2, destroy_func_3);
    SchedAddTask(sched, action_func_4, &a, 5, destroy_func_4);

	test(4, SchedSize(sched), "|| Size- 4");
	SchedRun(sched);
	SchedDestroy(sched);
}

static void test(int expected, int actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %d actual: %d\n", expected, actual);
    }
}

static int action_func_1(void *param)
{
	(void)param;	
	printf("Action Func 1 ");

	return 10;
}

static void destroy_func_1(void *param)
{
	(void)param;	
	
	printf("Destroy Func 1\n");
}

static int action_func_2(void *param)
{		
	SchedStop((scheduler_t*)param);	
	
	printf("Action Func 2 ");

	return 10;
}

static void destroy_func_2(void *param)
{
	(void)param;	
	
	printf("Destroy Func 2\n");
}
static int action_func_3(void *param)
{
	SchedRemoveTask((scheduler_t*)param, uid_to_remove);
	
	printf("Action Func 3 ");

	return 10;
}

static void destroy_func_3(void *param)
{
	(void)param;	
	
	printf("Destroy Func 3\n");
}
static int action_func_4(void *param)
{
	(void)param;	

	printf("Action Func 4 ");

	return 10;
}

static void destroy_func_4(void *param)
{
	(void)param;	

	printf("Destroy Func 4\n");
}

void CreateStopFile()
{
	FILE *file = fopen("rtstop.txt", "w");
	fputc(48, file);
	fclose(file);
}
/*
static int CheckRTStop(void *sched)
{
	FILE *file = fopen("rtstop.txt", "r");
	int user_input = 0;
	
	fscanf(file, "%d", &user_input);	
	fclose(file);

	if (1 == user_input)
	{
		((scheduler_t*)sched)->to_stop = 1;
	}

	return 1;
}
*/
