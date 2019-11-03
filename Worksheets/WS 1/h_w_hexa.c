****************************************************************************
//The program prints "Hello World!" by using hexadecimal numbers and ASCII//
****************************************************************************
#include <stdio.h>

void main()
{
 char* c = "\x22\x48\x65\x6C\x6C\x6F \x57\x6F\x72\x6C\x64\x21\x22";
 printf( "%s", c );
}
