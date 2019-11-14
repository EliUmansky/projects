/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function receivs 2 string that contain only
     digits and returns the sum of the numbers 
***************************************************/
#include <stdio.h>/*printf*/
#include <string.h>/*strlen*/
#include <stdlib.h>/*malloc*/


char *StrToNum ( char *str1, char *str2 );

int main ()
{
	char *str1 = "111111111111111111111111";
	char *str2 = "123456781234567812345678";
	printf("%s", StrToNum(str1, str2));

	return 0;
}

char *StrToNum ( char *str1, char *str2 )
{
	int tens = 0;
	int len = ( strlen(str1) > strlen(str2) )
	? strlen(str1) : strlen(str2) ;
	char *str3 = (char*)malloc(len + 2);
	
	char *str1_end = str1 + strlen(str1) - 1;
	char *str2_end = str2 + strlen(str2) - 1;
	char *str3_end = str3 + len ;
	
	*str3_end = '\0';
	str3_end--;
	
	while (str1_end >= str1 && str2_end >= str2 )
	{

		if ( (( *str1_end - 48 ) + ( *str2_end - 48 ) + tens ) < 10 )		
		{	
			
			*str3_end = (( *str1_end - 48 ) + ( *str2_end - 48 ) + tens) + 48;
			tens = 0;
			str1_end--;
			str2_end--;
			str3_end--;
		}
		else
		{
			
			*str3_end = (( ( *str1_end - 48 ) + ( *str2_end - 48 ) + tens ) % 10) +48;
			tens = 1;
			str1_end--;
			str2_end--;
			str3_end--;
		}
	}
	while (str1_end >= str1)
	{
		if ( (( *str1_end - 48 ) + tens ) < 10 )		
		{	
			*str3_end = (( *str1_end - 48 ) + tens) + 48;
			tens = 0;
			str1_end--;
			str3_end--;
		}
		else
		{
			*str3_end = (( ( *str1_end - 48 ) + tens ) % 10) + 48;
			tens = 1;
			str1_end--;
			str3_end--;
		}
	}
	while (str2_end >= str2)
	{
		if ( (( *str2_end - 48 ) + tens ) < 10 )		
		{	
			*str3_end = ( *str2_end - 48 ) + tens;
			tens = 0;
			str2_end--;
			str3_end--;
		}
		else
		{
			*str3_end = ( ( *str2_end - 48 ) + tens ) % 10;
			tens = 1;
			str2_end--;
			str3_end--;
		}
	}
	
    return str3;

}

