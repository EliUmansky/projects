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

	while (1) /* switch case loop*/
    {        	
		scanf("%c", &ch);

		character = (unsigned int) ch;

        switch (character)
        {
            case 65:
                printf("A pressed\n");
                break;
            case 84:
                printf("T pressed\n");
                break;
            case 27:
                system("stty icanon echo");
                return -1;
            default:
                break;
        }
    }
	
	return 0;
}
