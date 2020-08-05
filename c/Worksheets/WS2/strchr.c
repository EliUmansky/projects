/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function returns the pointer of the last
     occurence of a specified character
***************************************************/

#include <stdio.h> //printf

const char *Strchr (const char *str, char ch);

int main()
{
	const char *str = "I am hungry";
    char ch = 'h';
	const char *str2 = Strchr(str,ch);
	printf("%s", str2);

	return 0;
}
const char *Strchr (const char *str, char ch)
{
    const char *runner = str;

    while ( *runner )
    {
        runner++;
    }
	runner--;

	while (runner > str)
	{
		if ( *runner == ch )
		{
			return runner;
		}	
		runner--;
	}
	
	return 0;
}
