/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function concatenates two received strings 
***************************************************/
#include <stdio.h>//printf

void Strcat ( char *dest, char *source );
int Strlen (const char *str );
int main()
{
	char dest[50] = "I am hungry";
	char *source = " and sad";
	Strcat(dest, source);
	printf("%s", dest);

	return 0;
}
void Strcat ( char *dest, char *source )
{
	char *dest_run = dest + Strlen(dest);
	char *source_run = source;	
	
    while ( *source_run )
    {
        *dest_run++ = *source_run++;
    }
    *dest_run = '\0';
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
