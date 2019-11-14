/********************************************************
         Author: Eli Umansky, Date: 03.11.19
*********************************************************
This function receives 2 strings and a number and copies  
  that number of characters from one string to another
********************************************************/
#include <stdio.h>

void Strncpy (const char *source, char *dest, int num);

int main()
{
	const char *source = "I am hungry";
	char dest[20];
	int num = 6;
	Strncpy(source, dest, num);
	printf("%s", dest);

	return 0;
}
void Strncpy (const char *source, char *dest, int num)
{
	const char *runner = source;
    while ( num-- > 0 )
    {
        *dest++ = *runner++;
    }
    *dest = '\0';
}
