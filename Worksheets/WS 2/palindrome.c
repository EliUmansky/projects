/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
This function receives a string and returns 1 if
    the string is a palindrome, and 0 if not 
***************************************************/
#include <stdio.h>//printf

int Palindrome ( const char *str );
int main ()
{
  const char *str = "eli:4567654:ile";
  printf("%d", Palindrome(str));

   return(0);
}
int Palindrome ( const char *str )
{
    const char *start = str;
    while ( *str )
    {
        str++;
    }
    const char *end = str -1;
    while ( start++ < end-- )
    {
        if ( *start != *end )
        {
            return 0;
        }
    }
    if ( start >= end )
    {
        return 1;
    }
}
