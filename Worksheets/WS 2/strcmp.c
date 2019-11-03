/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function receives 2 strings and returns 1 if
     they are identical or 0 if they are not
***************************************************/
#include <stdio.h>

int Strcmp (const char *str1, const char *str2);

int main()
{
	const char *str1 = "I am Hungry";
	const char *str2 = "I am hungry";
    printf( "%d", Strcmp(str1,str2) );

	return 0;
}

int Strcmp (const char *str1, const char *str2)
{
    const char *runner1 = str1;
	const char *runner2 = str2;
    while ( *runner1 && *runner2 )
    {
        if ( *runner1 != *runner2 )
        {
            return 0;
        }
		runner1++;
		runner2++;
    }
    return ( '\0' == *runner1 && '\0' == *runner2 );
}
