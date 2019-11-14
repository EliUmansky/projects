//***************************************************************
//This program gets a number and outpts the result of 10^number//
//***************************************************************
#include <stdio.h>

float Pow( int num );

void main()
{
 int num;
 printf( "Enter a number: " );
 scanf("%d", &num );           //The program gets a number from the user
 printf( "10^%d is %f", num, Pow(num) );
}

float Pow( int num )     //The result of the func is multiplied by 10
{                        //as long as the input number is positive
 float res = 1;
 int temp = ( num > 0 ) ? num : -num ;  //we want temp to be positive
 while ( temp-- > 0 )
        {
		 res *= 10;
		}
 return ( num > 0 ) ? res : 1/res ;
}
       

