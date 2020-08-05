#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_num = 5;
int g_num2;


static int ReturnStaticInt()
{
	static int s_num = 6;

	++s_num;

	return s_num;
}

int AddNums(int num1, int num2)
{
	return (num1 + num2);
}

int AddLocalInts()
{
	const int num1 = 2;
	int num2 = 3;
	int res = 0;

	res = AddNums(num1, num2);
	printf("%d\n", res);

	return res;
}

int AddLocAndGlobInts1()
{
	int num1 = 2;
	int res = 0;

	res = AddNums(num1, g_num);
	printf("%d\n", res);

	return res;
}

int AddLocAndGlobInts2()
{
	int num1 = 2;
	int res = 0;

	res = AddNums(num1, g_num2);
	printf("%d\n", res);

	return res;
}

int AddLocAndStatInts()
{
	int num1 = 2;
	int res = 0;

	res = AddNums(num1, ReturnStaticInt());
	printf("%d\n", res);

	return res;
}


int AddAlocated()
{
	int *num1 = NULL;
	int *num2 = NULL;
	int *num3 = NULL;
	int res = 0;

	num1 = (int*)malloc(sizeof(int));
	num2 = (int*)malloc(sizeof(int));
	num3 = (int*)calloc(0x25000, 4);

	*num1 = 2;
	*num2 = 3;
	*num3 = 4;

	res = AddNums(*num1, *num2);
	
	printf("%d\n", res);

	free(num1);
	free(num2);
	free(num3);

	return res;
}

void StrCheck()
{
	char *str1 = "I'm tired";
	char *str2 = "aaaaaaaaaaa";
	char str3[100] = "hello";

	memcpy(str3, str2, 7);

	printf("%s", str3);
}

int main(int argc, char **argv, char** envp)
{
	char *str = "I'm tired";
	char** env;

    for(env=envp;*env!=0;env++)
    {
        char* thisEnv = *env;
        printf("%s\n",thisEnv);
    }


	AddLocalInts();
	AddLocAndGlobInts1();
	AddLocAndGlobInts2();
	AddLocAndStatInts();
	AddLocAndStatInts();
	AddAlocated();
	StrCheck();
	
	return 0;
}

