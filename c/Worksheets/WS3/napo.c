/********************************************************
         Author: Eli Umansky, Date: 07.11.19
*********************************************************
   This program solves Josephus problem and prints
        the location of the last man to stand
********************************************************/
#include <stdio.h> /*printf*/
#define LOCATION ( i % num )

int Last ( int arr[], int num );

int main ()
{
	int num = 100, i = 0;	
	int arr[100] = { 0 };
	
	for ( ; i < num; ++i )
	{
		arr[i] = 1;
	}
	printf( "%d\n", Last( arr, num ) );

	return 0;
}	

int Last ( int arr[], int num )	
{	
	int i = 0, count = 0;
	
	while ( count <= num )
	{
		do {
			++i;
			++count;
		} while ( 0 == *(arr + LOCATION ) );
	
		if ( count != num )
		{
			*(arr + LOCATION ) = 0;
			count = 0;
		}
		else
		{		
			return  LOCATION + 1;	
		}
			
		do {
			++i;
		} while ( 0 == *( arr + LOCATION ) );
	}	
}
