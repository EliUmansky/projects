/****************************
         Scedualer
    Test File - Integrated
    Author : Greg Markovsky
      Date : 11/12/2019   
****************************/

#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include <time.h> /* time_t */
#include <unistd.h> /* sleep */
#include <stdlib.h>

#include "tests_v06.h" /* TestStart */
#include "sched.h" /* PQCreate */
#include "task.h" /* task_t */

#define STOP_FILE ("StopFile")
#define ASCII_NUM_GAP (48)

typedef struct 
{
	time_t birth_time;
	size_t iteration;
	size_t repeat;
	size_t end;
	int voc;
	scheduler_t *sched;
	size_t task_num;
} test_task_t;

typedef struct
{
	scheduler_t *sched;
	ilrd_uid_t *task_id;
	size_t task_num;
} test_remove_t; 

static void TestSingleTaskExitEmpty(int voc);
static void TestSingleTaskExitStop(int voc);
static void TestFourTaskSync(int voc);
static void TestRunStopRunNoNew(int voc);
static void TestRunStopRunWithNewTask(int voc);
static void TestRealTimevsRunTime(int voc);
static void TestRemoveSelf(int voc);
static void TestStopWithFile(int voc);

static size_t ActTestSyncTillEnd(void *param);
static size_t ActTestSyncTillStop(void *param);
static size_t ActStop(void *param);
static void DesBasic(void *param);
static void DesTestTask(void *param);
static void TestSetTask(test_task_t *param, size_t start_offset, int voc, 
						scheduler_t *sched);
static size_t SchedSetStopFile(void *param);
static size_t ActRemoveSelf(void *id);

static int is_first = 1;

int main()
{
	/* pick SILENT or VERBOSE for additional data */

	RUN_TEST( VERBOSE , TestSingleTaskExitEmpty);
	RUN_TEST( VERBOSE , TestSingleTaskExitStop);
	RUN_TEST( VERBOSE , TestFourTaskSync);
	RUN_TEST( VERBOSE , TestRunStopRunNoNew);
	RUN_TEST( VERBOSE , TestRunStopRunWithNewTask);
	RUN_TEST( VERBOSE , TestRealTimevsRunTime);
	RUN_TEST( VERBOSE , TestRemoveSelf);
	RUN_TEST( VERBOSE , TestStopWithFile);

	return 0;
}

/* ----------------------------------------- Test Single Task Exit Empty --- */

static void TestSingleTaskExitEmpty(int voc)
{
	test_task_t param = { 0, 0, 2, 5, 0, 0, 1 };
	size_t start_offset = 4;
	scheduler_t *sched = NULL;
	char *test_conditions = "Single Repeating Task"; 

	TestStart("Single Task End on Empty", test_conditions);
	sched = SchedCreate();

	TestSetTask(&param, start_offset, voc, sched);

	SchedAddTask(sched, &ActTestSyncTillEnd, &param, start_offset, &DesTestTask);
	SchedRun(sched);
	SchedDestroy(sched); 
}

/* ------------------------------------------ Test Single Task Exit Stop --- */

static void TestSingleTaskExitStop(int voc)
{
	test_task_t param = { 0, 0, 2, 5, 0, 0, 1 };
	size_t start_offset = 4;
	scheduler_t *sched = NULL;
	char *test_conditions = "Single Repeating Task. Stop: stop signal"; 

	TestStart("Single Task End on Stop", test_conditions);
	sched = SchedCreate();

	TestSetTask(&param, start_offset, voc, sched);

	SchedAddTask(sched, &ActTestSyncTillStop, &param, start_offset, &DesTestTask);
	SchedRun(sched);
	SchedDestroy(sched); 
}

/* ------------------------------------------------ Test Four Task Sync --- */

static void TestFourTaskSync(int voc)
{
	test_task_t param_a = { 0, 0, 2, 100, 0, 0, 1 };
	test_task_t param_b = { 0, 0, 4, 100, 0, 0, 2 };
	test_task_t param_c = { 0, 0, 8, 100, 0, 0, 3 };
	size_t start_offset = 1;
	scheduler_t *sched = NULL;
	char *test_conditions = "Proper task sync"; 

	TestStart("Sync three tasks and stop", test_conditions);
	sched = SchedCreate();

	TestSetTask(&param_a, start_offset, voc, sched);
	TestSetTask(&param_b, start_offset, voc, sched);
	TestSetTask(&param_c, start_offset, voc, sched);

	SchedAddTask(sched, &ActTestSyncTillEnd, &param_a, start_offset, &DesTestTask);
	SchedAddTask(sched, &ActTestSyncTillEnd, &param_b, start_offset, &DesTestTask);
	SchedAddTask(sched, &ActTestSyncTillEnd, &param_c, start_offset, &DesTestTask);
	SchedAddTask(sched, &ActStop, sched, 18, &DesBasic);

	SchedRun(sched);
	SchedDestroy(sched); 
}

/* -------------------------------------------- Test Run-Stop-Run No New --- */

static void TestRunStopRunNoNew(int voc)
{
	test_task_t param = { 0, 0, 1, 100, 0, 0, 1 };
	size_t start_offset = 1;
	scheduler_t *sched = NULL;
	char *test_conditions = "Run-Stop-Run with no new tasks inbetween"; 

	TestStart("Run-Stop-Run, no new", test_conditions);
	sched = SchedCreate();

	TestSetTask(&param, start_offset, voc, sched);

	SchedAddTask(sched, &ActTestSyncTillEnd, &param, start_offset, &DesTestTask);
	SchedAddTask(sched, &ActStop, sched, 5, &DesBasic);
	SchedAddTask(sched, &ActStop, sched, 10, &DesBasic);

	SchedRun(sched);
	printf("[Second Run:]\n");
	SchedRun(sched);

	SchedDestroy(sched); 
}

/* ------------------------------------- Test Run-Stop-Run With New Task --- */

static void TestRunStopRunWithNewTask(int voc)
{
	test_task_t param_a = { 0, 0, 1, 100, 0, 0, 1 };
	test_task_t param_b = { 0, 0, 2, 100, 0, 0, 2 };
	size_t start_offset = 1;
	scheduler_t *sched = NULL;
	char *test_conditions = "Run-Stop-Run with a new task added inbetween"; 

	TestStart("Run-Stop-Run, with new task", test_conditions);
	sched = SchedCreate();

	TestSetTask(&param_a, start_offset, voc, sched);
	TestSetTask(&param_b, start_offset + 5, voc, sched);

	SchedAddTask(sched, &ActTestSyncTillEnd, &param_a, start_offset, &DesTestTask);
	SchedAddTask(sched, &ActStop, sched, 5, &DesBasic);
	SchedAddTask(sched, &ActStop, sched, 10, &DesBasic);

	SchedRun(sched);
	SchedAddTask(sched, &ActTestSyncTillEnd, &param_b, start_offset, &DesTestTask);
	printf("[Second Run:]\n");
	SchedRun(sched);

	SchedDestroy(sched); 
}

/* ------------------------------------------ Test Real Time vs Run Time --- */

static void TestRealTimevsRunTime(int voc)
{
	test_task_t param = { 0, 0, 2, 4, 0, 0, 1 };
	size_t start_offset = 4;
	scheduler_t *sched = NULL;
	char *test_conditions = "Task should be aligned to add time, not run time"; 

	(void)voc;
	TestStart("Real Time vs Run Time", test_conditions);
	sched = SchedCreate();

	TestSetTask(&param, start_offset, voc, sched);
	SchedAddTask(sched, &ActTestSyncTillEnd, &param, start_offset, &DesTestTask);
	sleep(2);
	SchedRun(sched);
	SchedDestroy(sched); 
}

/* -------------------------------------------------- Test Remove Itself --- */

static void TestRemoveSelf(int voc)
{
	test_task_t param = { 0, 0, 1, 100, 0, 0, 1 };
	test_remove_t param_b = { NULL, NULL, 0 };
	ilrd_uid_t *task_id = NULL;
	size_t start_offset = 1;
	scheduler_t *sched = NULL;
	char *test_conditions = "Task Runs then Self Removes"; 

	TestStart("Self Removing Task", test_conditions);
	sched = SchedCreate();

	TestSetTask(&param, start_offset, voc, sched);

	SchedAddTask(sched, &ActTestSyncTillEnd, &param, start_offset, &DesTestTask);
	task_id = SchedAddTask(sched, &ActRemoveSelf, &param_b, 2, &DesBasic);
	SchedAddTask(sched, &ActStop, sched, 7, &DesBasic);
	param_b.task_id = task_id;
	param_b.sched = sched;
	param_b.task_num = 2;

	SchedRun(sched);
	SchedDestroy(sched); 
}

/* ------------------------------------------------- Test Stop With File --- */

static void TestStopWithFile(int voc)
{
	test_task_t param = { 0, 0, 1, 10, 0, 0, 1 };
	test_task_t param_err = { 0, 0, 1, 1, 0, 0, 2 };
	size_t start_offset = 1;
	scheduler_t *sched = NULL;
	char *test_conditions = "Stop using file"; 

	TestStart("Stop via Outside File", test_conditions);
	sched = SchedCreate();

	TestSetTask(&param, start_offset, voc, sched);
	TestSetTask(&param_err, 0, voc, sched);

	SchedAddTask(sched, &ActTestSyncTillEnd, &param, start_offset, &DesTestTask);
	SchedAddTask(sched, &SchedSetStopFile, NULL, 4, &DesBasic);
	SchedAddTask(sched, &ActTestSyncTillEnd, &param_err, 7, &DesTestTask);

	SchedRun(sched);
	SchedDestroy(sched); 
}



/* ----------------------------- Stub Functions ---------------------------- */
/* ---------------------------------------------- Act Test Sync Till End --- */

static size_t ActTestSyncTillEnd(void *param)
{
	time_t curr_time = 0;
	time_t birth_time = ((test_task_t *)param)->birth_time;
	size_t iteration = ((test_task_t *)param)->iteration;
	size_t repeat = ((test_task_t *)param)->repeat;
	size_t end = ((test_task_t *)param)->end;
	int voc = ((test_task_t *)param)->voc;
	size_t task_num = ((test_task_t *)param)->task_num;

	time(&curr_time);

	if (end <= iteration)
	{
		return 0;
	}
	printf("[Task #%lu] ", task_num);
	TestSizeT(voc, "Scheduler: correct task timing", 
			  (size_t)(birth_time + (iteration * repeat)),
			  (size_t)(curr_time));

	++((test_task_t *)param)->iteration;
	
	return repeat; 
}

static size_t ActTestSyncTillStop(void *param)
{
	time_t curr_time = 0;
	time_t birth_time = ((test_task_t *)param)->birth_time;
	size_t iteration = ((test_task_t *)param)->iteration;
	size_t repeat = ((test_task_t *)param)->repeat;
	size_t end = ((test_task_t *)param)->end;
	int voc = ((test_task_t *)param)->voc;
	scheduler_t *sched = ((test_task_t *)param)->sched;
	size_t task_num = ((test_task_t *)param)->task_num;

	time(&curr_time);

	if (end <= iteration)
	{
		SchedStop(sched);
	}

	printf("[Task #%lu] ", task_num);
	TestSizeT(voc, "Scheduler: correct task timing", 
			  (size_t)(birth_time + (iteration * repeat)),
			  (size_t)(curr_time));

	++((test_task_t *)param)->iteration;
	
	return repeat; 
}

static size_t ActStop(void *param)
{
	SchedStop((scheduler_t *)param);

	return 0;
}

static void DesBasic(void *param)
{
	(void)param;
	printf("Task Was Destroyed\n");
}

static void DesTestTask(void *param)
{
	size_t task_num = ((test_task_t *)param)->task_num;

	printf("Task #%lu Was Destroyed\n", task_num);
}


static void TestSetTask(test_task_t *param, size_t start_offset, int voc, 
						scheduler_t *sched)
{  
	time(&(param->birth_time));
	param->birth_time += start_offset;
	param->voc = voc;
	param->sched = sched;
}

static size_t SchedSetStopFile(void *param)
{
	FILE *stop_file = NULL;

	stop_file = fopen(STOP_FILE, "w");
	if (NULL == stop_file)
	{
		return -1;
	}
	(void)param;
	fputc(49, stop_file);
	fclose(stop_file);

	return 0;
}

static size_t ActRemoveSelf(void *id)
{
	ilrd_uid_t *task_id = ((test_remove_t *)id)->task_id;
	scheduler_t *sched = ((test_remove_t *)id)->sched;
	size_t task_num = ((test_remove_t *)id)->task_num;

	SchedRemoveTask(sched, task_id);

	printf("[Task #%lu] ", task_num);
	TestInt(1, "Self Remove - First Appearance", 1, is_first);
	is_first = 0;

	return 2;
}









