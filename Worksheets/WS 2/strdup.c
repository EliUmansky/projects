/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function receives a string, duplicates it and
        returns a pointer to the new string 
***************************************************/
#include <stdio.h> //printf
#include <stdlib.h>//malloc

char *Strdup ( const char *str );
int Strlen (const char *str );

int main()
{
	const char *str = "I am hungry";
	printf("%s", Strdup(str));

	return 0;
}
char *Strdup ( const char *str )
{
	int len = 0;
	const char *runner1 = str;    
	char *dest = (char*)malloc( Strlen(str) + 1 );
    char *runner2 = dest;
	
	while ( *runner1 )
	{
		*runner2 = *runner1;
		runner1++;
		runner2++;
	}
    *runner2 = '\0';
    
	return dest;
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
