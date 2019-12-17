/***************************
   Generic Tests File
      Source File
   Author : Greg Markovsky
     Date : 26/11/2019            
***************************/

#include <stdio.h> /* printf */
#include <string.h> /* memcmp */
#include <sys/types.h> /* ssize_t */

#include "tests.h" /* PRINT_COLOR_RESET */

static size_t g_test_num = 1;
static size_t num_of_errors = 0;

/* ---------------------------------------------------------- Test Start --- */

void TestStart(const char *name, const char *text)
{
	printf("[Test #%lu - %s]\n", g_test_num, name);
	printf("[%s]\n", text);
	++g_test_num;
}


/* ----------------------------------------------------------- Test Test --- */

void TestRep()
{
	if (0 == num_of_errors)
	{
		PRINT_COLOR_SET_TO_GREEN
		printf("[SUCCESS] - ");
		PRINT_COLOR_RESET
		printf("Test #%lu\n", (g_test_num - 1));
	}
	else
	{
		PRINT_COLOR_SET_TO_RED
		printf("[FAILURE] - ");
		PRINT_COLOR_RESET
		printf("Test #%lu\n", (g_test_num - 1));			
	}

	num_of_errors = 0;

	NEW_LINE
} 

/* ------------------------------------------------------------ Test Int --- */

void TestInt(int voc_status, const char *str, int exp, int rec)
{
	if (exp == rec)
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_GREEN
			printf("[SUCCESS] - %s -", str);
			printf(" [expected: %d, recieved: %d]\n", exp, rec);
			PRINT_COLOR_RESET
		}
	}
	else
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_RED
			printf("[FAILURE] - %s -", str);
			printf(" [expected: %d, recieved: %d]\n", exp, rec);
			PRINT_COLOR_RESET
		}
		++num_of_errors;
	}
}

/* --------------------------------------------------------- Test Size_t --- */

void TestSizeT(int voc_status, const char *str, size_t exp, size_t rec)
{
	if (exp == rec)
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_GREEN
			printf("[SUCCESS] - %s -", str);
			printf(" [expected: %lu, recieved: %lu]\n", exp, rec);
			PRINT_COLOR_RESET
		}
	}
	else
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_RED
			printf("[FAILURE] - %s -", str);
			printf(" [expected: %lu, recieved: %lu]\n", exp, rec);
			PRINT_COLOR_RESET
		}
		++num_of_errors;
	}
}

/* -------------------------------------------------------- Test SSize_t --- */

void TestSSizeT(int voc_status, const char *str, ssize_t exp, ssize_t rec)
{
	if (exp == rec)
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_GREEN
			printf("[SUCCESS] - %s -", str);
			printf(" [expected: %ld, recieved: %ld]\n", exp, rec);
			PRINT_COLOR_RESET
		}
	}
	else
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_RED
			printf("[FAILURE] - %s -", str);
			printf(" [expected: %ld, recieved: %ld]\n", exp, rec);
			PRINT_COLOR_RESET
		}
		++num_of_errors;
	}
}

/* ----------------------------------------------------- Test Size_t Hex --- */

void TestSizeTHex(int voc_status, const char *str, size_t exp, size_t rec)
{
	if (exp == rec)
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_GREEN
			printf("[SUCCESS] - %s -", str);
			printf(" [expected: 0x%lx, recieved: 0x%lx]\n", exp, rec);
			PRINT_COLOR_RESET
		}
	}
	else
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_RED
			printf("[FAILURE] - %s -", str);
			printf(" [expected: 0x%lx, recieved: 0x%lx]\n", exp, rec);
			PRINT_COLOR_RESET
		}
		++num_of_errors;
	}
}

/* ------------------------------------------------------- Test Not NULL --- */

void TestNotNull(int voc_status, const char *str, void *rec)
{
	if (NULL != rec)
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_GREEN
			printf("[SUCCESS] - %s -", str);
			printf(" [%lx is not null]\n", (size_t)rec);
			PRINT_COLOR_RESET
		}
	}
	else
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_RED
			printf("[FAILURE] - %s -", str);
			printf(" [%lx is null]\n", (size_t)rec);
			PRINT_COLOR_RESET
		}
		++num_of_errors;
	}
}

/* ---------------------------------------------------------- Test Bytes --- */

void TestBytes(int voc_status, const char *str, 
				const char *exp, const char *rec, size_t bytes)
{
	size_t i = 0;


	if (0 == memcmp(exp, rec, bytes))
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_GREEN
			printf("[SUCCESS] - %s -", str);
			printf(" [expected: ");
			for (i = 0; i < bytes; ++i)
			{
				if ('\0' == exp[i])
				{
					printf("@");
				}
				printf("%c", exp[i]);
			}
			printf("] [recieved: ");
			for (i = 0; i < bytes; ++i)
			{
				if ('\0' == rec[i])
				{
					printf("@");
				}
				printf("%c", rec[i]);
			}
			printf("]\n");
			PRINT_COLOR_RESET
		}
	}
	else
	{
		if (VERBOSE == voc_status)
		{
			PRINT_COLOR_SET_TO_RED
			printf("[FAILURE] - %s -", str);
			printf(" [expected: ");
			for (i = 0; i < bytes; ++i)
			{
				if ('\0' == exp[i])
				{
					printf("@");
				}
				printf("%c", exp[i]);
			}
			printf("] [recieved: ");
			for (i = 0; i < bytes; ++i)
			{
				if ('\0' == rec[i])
				{
					printf("@");
				}
				printf("%c", rec[i]);
			}
			printf("]\n");
			PRINT_COLOR_RESET
		}
		++num_of_errors;
	}
}
