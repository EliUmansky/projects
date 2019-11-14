/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function adds a specified number of characters
       from one string to the second one 
***************************************************/
#include <stdio.h>//printf

void Strncat ( char *dest, char *source, int num );
int Strlen (const char *str );
int main()
{
	char dest[50] = "I am hungry";
	char *source = " and sad";
	int num = 4; 
	Strncat(dest, source, num);
	printf("%s", dest);

	return 0;
}

void Strncat ( char *dest, char *source, int num )
{
	char *dest_run = dest + Strlen(dest);
	char *source_run = source;	
	
    while ( num-- > 0 )
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
