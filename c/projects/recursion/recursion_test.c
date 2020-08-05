#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "slist.h" /* slist_node_t */
#include "stack.h" /* stack_t */
#include "recursion.h" /* function declarations */

static void test(int expected, int actual, const char* testName);
static void addresstest(size_t expected, size_t actual, const char* testName);

void TestFibonacci();
void TestFiboRecursion();
void TestStrLen();
void TestStrCmp();
void TestStrCpy();
void TestSlistFlip();
void TestStackSort();
void TestStrCat();
void TestStrStr();

int main()
{
	TestFibonacci();
	TestFiboRecursion();
	TestStrLen();
	TestStrCmp();
	TestStrCpy();
	TestSlistFlip();
	TestStackSort();
	TestStrCat();
	TestStrStr();

	return 0;
}

void TestFibonacci()
{
	printf("Normal Fibonacci Tests\n");	
	printf("----------------------\n");
	test(1, Fibonacci(0), "Fibonacci 0 = 1");
	test(1, Fibonacci(1), "Fibonacci 1 = 1");
	test(2, Fibonacci(2), "Fibonacci 2 = 2");
	test(8, Fibonacci(5), "Fibonacci 5 = 8");
	test(21, Fibonacci(7), "Fibonacci 7 = 21");
}	

void TestFiboRecursion()
{
	printf("\nRecursive Fibonacci Tests\n");	
	printf("---------------------------\n");

	test(1, RecFibonacci(0), "Fibonacci 0 = 1");
	test(1, RecFibonacci(1), "Fibonacci 1 = 1");
	test(2, RecFibonacci(2), "Fibonacci 2 = 2");
	test(8, RecFibonacci(5), "Fibonacci 5 = 8");
	test(21, RecFibonacci(7), "Fibonacci 7 = 21");
}

void TestStrLen()
{
	printf("\nStrLen Tests\n");	
	printf("--------------\n");

	test(0, RecStrLen(""), "StrLen = 0");
	test(1, RecStrLen("y"), "StrLen y = 1");
	test(5, RecStrLen("hello"), "StrLen hello = 2");
	test(10, RecStrLen("I'm hungry"), "StrLen I'm hungry = 8");
}

void TestStrCmp()
{
	char *str1 = "hello";
	char *str2 = "";
	char *str3 = "HELLO";
	char *str4 = "abcd";
	char *str5 = "dcba";
	char *str6 = "abcdefg";
	char *str7 = "abce";

	printf("\nStrCmp Tests\n");	
	printf("--------------\n");
	
	test(RecStrCmp("", ""), strcmp(str2, str2), "Empty strings");
	test(RecStrCmp("hello", "hello"), strcmp(str1, str1), "Equal strings");
	test(RecStrCmp("hello", "HELLO"), strcmp(str1, str3), "Not equal 1");
	test(RecStrCmp("abcd", "dcba"), strcmp(str4, str5), "Not equal 2");
	test(RecStrCmp("abcdefg", "abcd"), strcmp(str6, str4), "Not equal 3");
	test(RecStrCmp("abcd", "abce"), strcmp(str4, str7), "Not equal 4");
}

void TestStrCpy()
{
	char str1[10] = "hello";
	char str2[10] = "world";
	char str3[10] = "abc";
	char str4[10] = "abcdefg";
	char str5[10] = "";
	char str11[10] = "hello";
	char str12[10] = "world";
	char str13[10] = "abc";
	char str14[10] = "abcdefg";
	char str15[10] = "";

	printf("\nStrCpy Tests\n");	
	printf("--------------\n");

	test(0, RecStrCmp(strcpy(str11, str12), RecStrCpy(str1, str2)), "Copy same size");
	test(0, RecStrCmp(strcpy(str13, str12), RecStrCpy(str3, str2)), "Copy larger to smaller");
	test(0, RecStrCmp(strcpy(str14, str12), RecStrCpy(str4, str2)), "Copy smaller to larger");
	test(0, RecStrCmp(strcpy(str14, str15), RecStrCpy(str4, str5)), "Copy empty to not empty");
}

void TestSlistFlip()
{
	int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7, i = 0;
	slist_node_t *node1 = SlistCreateNode(&a, NULL);
	slist_node_t *node2 = SlistCreateNode(&b, node1);
	slist_node_t *node3 = SlistCreateNode(&c, node2);
	slist_node_t *node4 = SlistCreateNode(&d, node3);
	slist_node_t *node5 = SlistCreateNode(&e, node4);
	slist_node_t *node6 = SlistCreateNode(&f, node5);
	slist_node_t *node7 = SlistCreateNode(&g, node6);
	slist_node_t *test_node = node7;
	slist_node_t *new_first = NULL;

	printf("\nSlistFlip Tests\n");	
	printf("-----------------\n");

	for (i = 6; i >= 0; --i)
	{
		test(i + 1, *(int*)(test_node->data), "Checking correct positions");
		test_node = test_node->next;
	}
	printf("\n");	

	new_first = RecSlistFlip(node7);
	test_node = node1;

	test(*(int*)(test_node->data), *(int*)(new_first->data), "New head\n");

	for (i = 0; i < 7; ++i)
	{
		test(i + 1, *(int*)(test_node->data), "Checking positions after flip");
		test_node = test_node->next;
	}

	SlistFreeAll(node1);
}

void TestStackSort()
{
	stack_t *stack = NULL;
	int arr[] = { 7, 9, 3, 2, 8, 6, 4 };
	int sorted_arr[] = { 2, 3, 4, 6, 7, 8, 9 };
	int i = 0;

	printf("\nStackSort Tests\n");	
	printf("-----------------\n");

	stack = StackCreate(7, sizeof(int));
	
	for (; i < 7; ++i)
	{
		StackPush(stack, arr + i);
	}

	RecStackSort(stack);

	for (i = 0; i < 7; ++i)
	{
		test(*(sorted_arr + i), *(int*)StackPeek(stack), "Checking order of elements after sort");
		StackPop(stack);
	}
}

void TestStrCat()
{
	char str1[] = "I'm tired ";
	char str2[] = "and hungry";
	char str3[] = "I'm tired ";
	char str4[] = "and hungry";

	printf("\nStrCat Tests\n");	
	printf("--------------\n");

	test(0, RecStrCmp(RecStrCat(str1, str2), strcat(str3, str4)), "Strcat");
}

void TestStrStr()
{
	char str1[] = "hello world";
	char str2[] = "hel";
	char str3[] = "or";
	char str4[] = "z";
	char *str5;

	printf("\nStrStr Tests\n");	
	printf("--------------\n");

	str5 = RecStrStr(str1, str2);
	printf("hello world = %s\n", str5);
	str5 = RecStrStr(str1, str3);
	printf("orld = %s\n", str5);
	str5 = RecStrStr(str1, str4);
	addresstest((size_t)NULL, (size_t)str5, "Returns NULL if not found");
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

static void addresstest(size_t expected, size_t actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %ld actual: %ld\n", expected, actual);
    }
}
