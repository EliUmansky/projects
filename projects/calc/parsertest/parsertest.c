#include <stdio.h>

#include "parser.h"

static void test(float expected, float actual, const char* testName);
static void chartest(char expected, char actual, const char* testName);

int main()
{
	char *str = "4+5.2-6*2";
	char *str2 = "-4*5.2-6*-2";

	test(4.0, GetNum(str, &str), "Getnum");
	chartest('+', GetOperator(&str), "GetOperator");
	test(5.2, GetNum(str, &str), "Getnum");
	chartest('-', GetOperator(&str), "GetOperator");
	test(6.0, GetNum(str, &str), "Getnum");
	chartest('*', GetOperator(&str), "GetOperator");
	test(2.0, GetNum(str, &str), "Getnum\n");

	test(-4.0, GetNum(str2, &str2), "Getnum");
	chartest('*', GetOperator(&str2), "GetOperator");
	test(5.2, GetNum(str2, &str2), "Getnum");
	chartest('-', GetOperator(&str2), "GetOperator");
	test(6.0, GetNum(str2, &str2), "Getnum");
	chartest('*', GetOperator(&str2), "GetOperator");
	test(-2.0, GetNum(str2, &str2), "Getnum");

	return 0;
}

static void test(float expected, float actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %f actual: %f\n", expected, actual);
    }
}

static void chartest(char expected, char actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %c actual: %c\n", expected, actual);
    }
}
