#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

double GetNum(char *str, char **updated)
{
	return strtod(str, updated);
}

char GetOperator(char **str)
{
	char result = **str;

	++*str;

	return result;
}
	

