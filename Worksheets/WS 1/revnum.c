*******************************************
//This program gets a number and flips it//
*******************************************
#include <stdio.h>

int RevNum (int num);

void main()
{
 int num;
 printf( "Enter a number: " );
 scanf( "%d", &num );           //The program gets a number from the user
 printf( "%d\n", RevNum(num) );
}

int RevNum (int num)           //The function flips the number
{
 int rev = 0;

 while ( num / 10 > 0 || num % 10 > 0)
      {
       rev = 10 * rev + (num % 10 );
       num /= 10;
      }
 return rev;
}
