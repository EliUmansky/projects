/***************************************
* Name: Jenny Kogan                    *
* Date: 07/01/20                       *
* Exercise - Calculator - Convertor    *
****************************************/
#include <stdlib.h>

#include "convertor.h" /* convertorfuncs */

double GetNumConvertor(char *expression, char **next)
{
	double num = 0;
	
	num = strtod(expression, next);

	return num;
}

char GetOperatorConvertor(char **expression)
{
	*expression += 1;
	
	return *(*expression - 1);
}
