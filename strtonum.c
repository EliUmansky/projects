/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function receivs 2 string that contain only
     digits and returns the sum of the numbers 
***************************************************/
#include <stdio.h>/*printf*/
#include <string.h>/*strlen*/

int len ( char *str );
int StrToNum ( char *str1, char *str2 );

int main ()
{
	char str1[] = "98";
	char str2[] = "135";

	printf("%d\n", StrToNum (str1, str2));
	

	return 0;
}

int StrToNum ( char *str1, char *str2 )
{
	int tens = 1;
	int sum = 0;
	char *str1_end = str1 + len(str1) - 1;
	char *str2_end = str2 + len(str2) - 1;
	
	while (str1_end >= str1 && str2_end >= str2 )
	{
		sum += tens * ( *str1_end - 48 ) + tens * ( *str2_end - 48 );
		tens *= 10;
		str1_end--;
		str2_end--;
	}
	while (str1_end >= str1)
	{
		sum += tens * ( *str1_end - 48 );
		tens *= 10;
		str1_end--;
	}
	while (str2_end >= str2)
	{
		sum += tens * ( *str2_end - 48 );
		tens *= 10;
		str2_end--;
	}
	
    return sum;

}
int len ( char *str )
{
    char *runner = str;
    while ( *runner )
    {
        runner++;
    }
    return runner - str;
}
