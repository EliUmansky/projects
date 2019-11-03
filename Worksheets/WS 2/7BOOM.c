/*****************************************************
        Author: Eli Umansky, Date: 03.11.19
******************************************************
This function receives 2 numbers and prints them, 
except for numbers that contain the digit 7 or are 
divisible by 7 with no remainder (prints BOOM instead)
*****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Seven_Boom ( int num1, int num2 );
int main ()
{
    int num1 = 1;
    int num2 = 70;
    Seven_Boom(num1, num2);

   return(0);
}
void Seven_Boom ( int num1, int num2 )
{
    int i = 0;
    for ( i = num1; i <= num2; i++ )
    {
        int t_num = i;
        while ( t_num )
        {
            if ( t_num % 10 == 7 )
            {
                break;
            }
            t_num /= 10;
        }
        if ( t_num == 0 && i % 7 != 0 )
        {
            printf("%d ", i);
        }
        else
        {
            printf("BOOM ");
        }
    }
}
