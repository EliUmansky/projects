#include <stdio.h> /* size_t */
#include <sys/types.h> /* ssize_t */
#include <stdlib.h> /* free */
#include <string.h> /* strncmp */
#include "cbuff.h"

typedef char byte; 

struct cbuff
{
	byte *start;
	size_t capacity;
	size_t read_index;
	size_t size;
};

static void test(int expected, int actual, const char* testName);

void TestFlow1();
void TestFlow2();
void TestFlow3();
void TestFlow4();
void TestFlow5();
/*void TestFlow6();
void TestFlow7();
*/
int main()
{
	
	TestFlow1();
	TestFlow2();
	TestFlow3();
	TestFlow4();
	TestFlow5();
/*	TestFlow6();
	TestFlow7();
*/
	return 0;
}

void TestFlow1()
{
	cbuff_t *buffer = CbuffCreate(20);
	CbuffDestroy(buffer);
}

void TestFlow2()
{
	cbuff_t *buffer = CbuffCreate(20);

	puts("Flow2");		
	
	test(20, CbuffFreeSpace(buffer), "Free Space");
	test(1, CbuffIsEmpty(buffer), "IsEmpty");
	CbuffDestroy(buffer);	
	puts(" "); 
}

void TestFlow3()
{
	char str1[] = "I'm hungry";
	char str2[] = "And tired";
	
	cbuff_t *buffer = CbuffCreate(30);

	puts("Flow3");		

	test(30, CbuffFreeSpace(buffer), "Free Space");

	CbuffWrite(buffer, str1, 5);

	test(25, CbuffFreeSpace(buffer), "Free Space");

	CbuffWrite(buffer, str2, 20);

	test(5, CbuffFreeSpace(buffer), "Free Space");

	CbuffWrite(buffer, str2, 10);

	test(0, CbuffFreeSpace(buffer), "Free Space");
	
	puts(" "); 
}

void TestFlow4()
{
	char str1[] = "I'm hungry"; /* 11 */
	char str2[] = "And tired"; /* 10 */
	char str3[] = "Veeeery tired"; /* 14 */
	char data[] = " ";
	ssize_t copied_bytes = 0;

	cbuff_t *buffer = CbuffCreate(40);

	puts("Flow4");		

	CbuffWrite(buffer, str1, 0);

	test(40, CbuffFreeSpace(buffer), "Free Space after copying 0 bytes");

	copied_bytes = CbuffWrite(buffer, str1, 11);

	test(11, copied_bytes, "Number of copied bytes");
	test(29, CbuffFreeSpace(buffer), "Free Space after writing");

	CbuffWrite(buffer, str2, 10);
	CbuffWrite(buffer, str2, 8);

	test(11, CbuffFreeSpace(buffer), "Free Space after writing");
	
	CbuffRead(buffer, data, 11);
	
	test(22, CbuffFreeSpace(buffer), "Free Space after reading");
	
	test(0, strncmp(str1, data, 11), "String compare");

	CbuffWrite(buffer, str3, 14);

	test(8, CbuffFreeSpace(buffer), "Free Space after full loop");	

	puts(" "); 
}

void TestFlow5()
{
	char str1[] = "I'm hungry"; /* 11 */
	char str2[] = "And tired"; /* 10 */
	char str3[] = "Veeeery tired"; /* 14 */
	char str4[] = "1234512345123451234512345123451234512345"; /* 41 */	
	char data[] = " ";
	ssize_t copied_bytes = 0;

	cbuff_t *buffer = CbuffCreate(40);

	puts("Flow4");		

	CbuffWrite(buffer, str4, 50);

	test(0, CbuffFreeSpace(buffer), "Free Space after writing");

	CbuffRead(buffer, data, 50);
	test(0, strncmp(str4, data, 39), "String compare");
	printf("%s\n", data);
	printf("%s", str4);
	puts(" "); 
}
/*
void TestFlow6()
{
	const int arr[] = {1, 2, 3, 4, 5, 6};	
	queue_t *queue1 = QueCreate();
	queue_t *queue2 = QueCreate();
	int i = 0;

	puts("Flow6");	

	for(; i < 6; ++i)
	{
		QueEnqueue(queue1, arr + i);
	}

	queue1 = QueAppend(queue1, queue2);

	test(6, QueSize(queue1), "Size");
	test(1, *(int*)QuePeek(queue1), "Peek");

	QueDequeue(queue1);
	QueDequeue(queue1);
	QueDequeue(queue1);

	test(3, QueSize(queue1), "Size");
	test(4, *(int*)QuePeek(queue1), "Peek");

	QueDestroy(queue1);
	puts(" ");
}

void TestFlow7()
{
	const int arr[] = {1, 2, 3, 4, 5, 6};	
	queue_t *queue1 = QueCreate();
	queue_t *queue2 = QueCreate();
	int i = 0;

	puts("Flow7");	

	for(; i < 6; ++i)
	{
		QueEnqueue(queue2, arr + i);
	}

	queue1 = QueAppend(queue1, queue2);

	test(6, QueSize(queue1), "Size");
	test(1, *(int*)QuePeek(queue1), "Peek");

	QueDequeue(queue1);
	QueDequeue(queue1);
	QueDequeue(queue1);

	test(3, QueSize(queue1), "Size");
	test(4, *(int*)QuePeek(queue1), "Peek");

	QueDestroy(queue1);
	puts(" ");
}
*/
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

