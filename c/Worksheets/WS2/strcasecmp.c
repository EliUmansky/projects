#include <stdio.h> //printf
#include <ctype.h> //isupper islower
int Strcasecmp (const char *str1, const char *str2);

int main()
{
	const char *str1 = "I AM hungry";
	const char *str2 = "I am HuNgrY";

	printf("%d", Strcasecmp(str1, str2));

	return 0;
}

int Strcasecmp (const char *str1, const char *str2)
{
    const char *runner1 = str1;
    const char *runner2 = str2;
	int diff = 32;              // in ASCII table: a - A = 32
	
	while ( *runner1 && *runner2 )
	{
		if ( isupper(*runner1) && islower(*runner2) )
		{
			if ( (*runner1 + diff) != *runner2 )
			{
				return 0;
			}
		}
		else if ( isupper(*runner2) && islower(*runner1) )
		{
			if ( (*runner2 + diff) != *runner1 )
			{
				return 0;
			}
		}
		else
		{
			if ( *runner2 != *runner1 )
			{
				return 0;
			}
		}
		runner1++;
		runner2++;
	}
	
	return ( '\0' == *runner1 && '\0' == *runner2 );
}
