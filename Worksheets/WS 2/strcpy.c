/********************************************************
         Author: Eli Umansky, Date: 03.11.19
*********************************************************
This function receives 2 strings and copies the content
          of one string to the second one
********************************************************/
#include <stdio.h>
#include <assert.h>

void Strcpy (const char *source, char *dest);

int main()
{
	const char *source = "I am hungry";
	char dest[20];
	Strcpy(source, dest);
	printf("%s", dest);
	return 0;
}

void Strcpy (const char *source, char *dest)
{
	const char *runner = source;
	assert ( *runner );
    while ( *runner )
    {
     	*dest++ = *runner++;
    }
    *dest = '\0';
}
