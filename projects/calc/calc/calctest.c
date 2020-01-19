#include <stdio.h>

#include "calc.h"

enum status {SUCCESS, MALLOC_FAIL, WRONG_INPUT, DIVIDE_BY_ZERO, NEGATIVE_ROOT, 
					NUM_OF_ERRORS};

static void test(double expected, double actual, const char* testName);
static void failtest(int expected, int actual, const char* testName);
void TwoNums();
void ThreeNums();
void MoreNums();
void Fails();
void Parentheses();

int main()
{
	TwoNums();
	ThreeNums();
	MoreNums();
	Parentheses();
	Fails();

	return 0;
}

void TwoNums()
{
	char *str1 = "3+2";
	char *str2 = "3-2";
	char *str3 = "3*2";
	char *str4 = "3/2";
	char *str5 = "3^2";

	double ans = 0;
	int status = 0;

	printf("Test 1 - Two Numbers (Simple Calculations)\n");

	ans = Calc(str1, &status);
	test(5, ans, "3+2");	
	ans = Calc(str2, &status);
	test(1, ans, "3-2");
	ans = Calc(str3, &status);
	test(6, ans, "3*2");
	ans = Calc(str4, &status);
	test(1.5, ans, "3/2");
	ans = Calc(str5, &status);
	test(9, ans, "3^2");	
}

void ThreeNums()
{
	char *str1 = "3+2*4";
	char *str2 = "3-2/4";
	char *str3 = "3*2+4";
	char *str4 = "3/2-4";
	char *str5 = "3^2*4";
	char *str6 = "3+2+4";

	double ans = 0;
	int status = 0;

	printf("\nTest 2 - Three Numbers (Simple Calculations)\n");

	ans = Calc(str1, &status);
	test(11, ans, "3+2*4");	
	ans = Calc(str2, &status);
	test(2.5, ans, "3-2/4");
	ans = Calc(str3, &status);
	test(10, ans, "3*2+4");
	ans = Calc(str4, &status);
	test(-2.5, ans, "3/2-4");
	ans = Calc(str5, &status);
	test(36, ans, "3^2*4");	
	ans = Calc(str6, &status);
	test(9, ans, "3+2+4");	
}

void MoreNums()
{
	char *str1 = "3+2*4+2*3";
	char *str2 = "3-2/4*5-1";
	char *str3 = "3*2+4^2";
	char *str4 = "3/2+2^3^2";
	char *str5 = "2^2^1^2";
	char *str6 = "-3-2*4+1^6";

	double ans = 0;
	int status = 0;

	printf("\nTest 3 - Complex Calculations\n");

	ans = Calc(str1, &status);
	test(17, ans, "3+2*4+2*3");	
	ans = Calc(str2, &status);
	test(-0.5, ans, "3-2/4*5-1");
	ans = Calc(str3, &status);
	test(22, ans, "3*2+4^2");
	ans = Calc(str4, &status);
	test(513.5, ans, "3/2+2^3^2");
	ans = Calc(str5, &status);
	test(4, ans, "2^2^1^2");	
	ans = Calc(str6, &status);
	test(-10, ans, "-3-2*3+1^6");	
}

void Parentheses()
{
	char *str1 = "(3+5)";
	char *str2 = "3+(2*3)";
	char *str3 = "(2+2)*(1+3)";
	char *str4 = "2*{2+(2*[2+2])}";
	char *str5 = "(1*3)*2+{2+(2*[2+2])}";

	double ans = 0;
	int status = 0;

	printf("\nTest 4 - Parentheses\n");

	ans = Calc(str1, &status);
	test(8, ans, "(3+5)");	
	ans = Calc(str2, &status);
	test(9, ans, "3+(2*3)");
	ans = Calc(str3, &status);
	test(16, ans, "(2+2)*(1+3)");
	ans = Calc(str4, &status);
	test(20, ans, "2*{2+(2*[2+2])}");
	ans = Calc(str5, &status);
	test(16, ans, "(1*3)*2+{2+(2*[2+2])}");	
}

void Fails()
{
	char *str1 = "$+5";
	char *str2 = "3&2";
	char *str3 = "3/0";
	char *str4 = "-4^0.5";
	char *str5 = "((2+5)*4";
	char *str6 = "(2+3]*{2+2)";
	char *str7 = "x+4";

	int status = 0;

	printf("\nTest 5 - Fail Codes\n");

	Calc(str1, &status);
	failtest(WRONG_INPUT, status, "$+5");	
	status = 0;
	Calc(str2, &status);
	failtest(WRONG_INPUT, status, "3&2");
	status = 0;
	Calc(str3, &status);
	failtest(DIVIDE_BY_ZERO, status, "3/0");
	status = 0;
	Calc(str4, &status);
	failtest(NEGATIVE_ROOT, status, "-4^0.5");
	status = 0;
	Calc(str5, &status);
	failtest(WRONG_INPUT, status, "((2+5)*4");	
	status = 0;
	Calc(str6, &status);
	failtest(WRONG_INPUT, status, "(2+3]*{2+2)");
	status = 0;
	Calc(str7, &status);
	failtest(WRONG_INPUT, status, "x+4");
}

static void test(double expected, double actual, const char* testName)
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
static void failtest(int expected, int actual, const char* testName)
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
