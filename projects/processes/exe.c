#include<stdio.h> 
#include<unistd.h> 
  
int main() 
{ 
    printf("I am EXEC.c called by execvp() "); 
    printf("\n"); 
    sleep(3);
    return 0; 
} 
