/*************************
       Calculator
      Header File
    Author : OL80
      Date : 07/01/2020
*************************/

#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <stddef.h> /* size_t */

double GetNumConvertor(char *expression, char **next);

char GetOperatorConvertor(char **expression);

#endif /* CONVERTOR_H */
