/********************************************************
         Author: Eli Umansky, Date: 10.11.19
*********************************************************
   This program gets an input from the user, prints 
    'T' and 'A' if that's the input and exits the 
            program if the input is 'esc'
********************************************************/
#include <stdio.h> /*printf*/
#include <stdlib.h> /*system() + exit()*/

void FuncA (unsigned int ch);
void FuncT (unsigned int ch);
void FuncExit (unsigned int ch);
void FuncEmpty (unsigned int ch);

int main()
{
    char character = '\0';
    int i = 0;
    
	void (*ascii[256])(unsigned int); /*A function pointer to every ASCII char*/

    for (; i<256; ++i)
    {
        ascii[i] = FuncEmpty;
    }
    ascii[65] = FuncA;
    ascii[84] = FuncT;
    ascii[27] = FuncExit;

    system("stty -icanon -echo");
   
    while (1)
    {
        scanf("%c", &character);

        ascii[(unsigned int)character](character);

    }
}

void FuncA (unsigned int character) /*If the input is 'A'*/
{
    printf("A pressed\n");
}

void FuncT (unsigned int character) /*If the input is 'T'*/
{
    printf("T pressed\n");
}

void FuncExit (unsigned int character) /*If the input is 'esc'*/
{
    system("stty icanon echo");
    exit(0);
}

void FuncEmpty (unsigned int character) /* Other input*/
{

}


