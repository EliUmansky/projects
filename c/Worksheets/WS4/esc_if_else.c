/********************************************************
         Author: Eli Umansky, Date: 10.11.19
*********************************************************
   This program gets an input from the user, prints 
    'T' and 'A' if that's the input and exits the 
            program if the input is 'esc'
********************************************************/
#include<stdio.h> /*printf*/
#include<stdlib.h> /*system()*/

int main()
{
    char ch = '\0';
	unsigned int character = 0;

	system("stty -icanon -echo");
    
	while (1) /* if else loop*/
    {
        scanf("%c", &ch);
		
		character = (unsigned int) ch;

        if (84 == character)
        {
            printf("T pressed\n");
        }
        else if (65 == character)
        {
            printf("A pressed\n");
        }
        else if (27' == character)
        {
            system("stty icanon echo");
            return -1;
        }

    }

	return 0;
}
