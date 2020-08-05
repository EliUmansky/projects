//***************************************************
//This program swaps two integers by using pointers//
//***************************************************

#include <stdio.h>

void swap( int *pnum1, int *pnum2 );

int main()
{
 	int num1 = 2;
 	int num2 = 3;

 	swap( &num1, &num2 );
 	return 0;
}

void swap( int *pnum1, int *pnum2 ) //The funcion swaps the values stored in
{                                   //the addresses of integers num1 and num2
 	int temp = *pnum1;
    *pnum1 = *pnum2;
    *pnum2 = temp;
}
