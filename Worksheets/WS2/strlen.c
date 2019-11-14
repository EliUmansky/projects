/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function receives a string as an argument
            and returns its length
***************************************************/
#include <stdio.h>

int Strlen (const char *str );

int main()
{
	const char *str = "I am hungry";
    printf( "%d", Strlen(str) );

	return 0;
}

int Strlen (const char *str )
{
    const char *runner = str;
    while ( *runner )
    {
        runner++;
    }
    return runner - str;
}
