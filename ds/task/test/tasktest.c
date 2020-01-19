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
void TestFlow1();

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
