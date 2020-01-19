#include <stdio.h> /* size_t */
#include "uid.h"

static void test(int expected, int actual, const char* testName);
/*static void strtest(char *expected, char *actual, const char* testName);
static int IsPriorityBefore(const void *new_data, const void *data, void *param);
static int DataPlusParam(void *data, void *param);
static int FindInt(const void *data, const void *param);
*/
void TestFlow1();
void TestFlow2();
void TestFlow3();
void TestFlow4();
void TestFlow5();
void TestFlow6();
void TestFlow7();

int main()
{

	TestFlow1();
/*	TestFlow2();
	TestFlow3();
	TestFlow4();
	TestFlow5();
	TestFlow6();
	TestFlow7();
*/	
	return 0;
}

void TestFlow1()
{
	ilrd_uid_t uid1 = {0};	
	ilrd_uid_t uid2 = {0};	
	test(1, UIDIsSame(uid1, g_bad_uid), "|| IsBad- 1");
	
	uid1 = UIDCreate();
	test(0, UIDIsSame(uid1, g_bad_uid), "|| IsBad- 0\n");

	printf("Time is: %s", ctime(&uid1.time));
	printf("Pid is: %u\n", uid1.pid);
	printf("Counter is: %lu\n\n", uid1.counter);

	uid2 = UIDCreate();
	test(0, UIDIsSame(uid2, g_bad_uid), "|| IsBad- 0");

	printf("Time is: %s", ctime(&uid2.time));
	printf("Pid is: %u\n", uid2.pid);
	printf("Counter is: %lu\n\n", uid2.counter);

	test(0, UIDIsSame(uid1, uid2), "|| IsSame- 0");
	puts(" ");
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

