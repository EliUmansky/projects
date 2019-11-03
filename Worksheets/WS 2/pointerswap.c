/***************************************************
        Author: Eli Umansky, Date: 03.11.19
****************************************************
       This function swaps 2 pointers to int
***************************************************/
#include <stdio.h>//printf 

void Swap ( int **pp_num1, int **pp_num2 );
int main ()
{
    int num1 = 2;
    int num2 = 3;
    int *p_num1 = &num1;
    int *p_num2 = &num2;
    
    printf("&num1: %p, &num2: %p, p_num1: %p, p_num2: %p\n",
    &num1,&num2,p_num1,p_num2);
    Swap(&p_num1, &p_num2);
    printf("&num1: %p, &num2: %p, p_num1: %p, p_num2: %p\n",
    &num1,&num2,p_num1,p_num2);

   return(0);
}
void Swap ( int **pp_num1, int **pp_num2 )
{
    int *temp = *pp_num1;
    *pp_num1 = *pp_num2;
    *pp_num2 = temp;
}
