/***************************************************
        Author: Eli Umansky, Date: 04.11.19
****************************************************
This function removes whitespace characters from the
    beginning and end of a string and removes 
       duplicate whitespace characters 
***************************************************/
#include <stdio.h>//printf
#include <ctype.h>//isspace

int Strlen ( char *str );
void RmSpaces (char *str);
int main ()
{
    char str[] = "   eli\n\n\nha\t\tnasih  ";
    printf("%s", str);
    printf("\n");
    RmSpaces(str);
    printf("%s", str);

   return(0);
}
void RmSpaces (char *str)
{
	char *runner = str;
	char *subrunner = runner;

	while ( isspace(*runner) != 0 )  //removes whitespaces at the beginning
	{
		subrunner = runner;		
		while ( *subrunner != '\0' )
		{
			*subrunner = *(subrunner + 1);
			subrunner++;
		}
	}   
	
	while ( *runner != '\0' )  //removes duplicate whitespaces
	{
		if ( isspace(*runner) != 0 && *runner == *(runner + 1) )
		{
			subrunner = runner;
			while ( *subrunner != '\0' )
		    {
				*subrunner = *(subrunner + 1);
				subrunner++;
			}
			runner--;
		}
		runner++;
	}
	
	subrunner = str + Strlen(str) - 1;

    while ( isspace(*subrunner) != 0 ) //removes whitespaces at the end
	{
		*subrunner = '\0';
        subrunner--;
	}   
}
int Strlen ( char *str )
{
    char *runner = str;
    while ( *runner )
    {
        runner++;
    }
    return runner - str;
}
