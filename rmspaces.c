#include <stdio.h>
#include <string.h>
#include <ctype.h>
void RmSpaces (char *str);
int main ()
{
    char *str = "  eli\n\n\nha\t\tnasih  ";
    printf("%s", str);
    printf("\n");
    RmSpaces(str);
    printf("%s", str);

   return(0);
}
void RmSpaces (char *str)
{
    int i = 0, j = 0;
    while ( isspace( *(str + i) ) )
    {
        for ( j = 0; j < strlen(str); j++)
        {
            *(str + j) = *(str + j + 1);
        }
        i = 0;
    }
    for ( i = 0; i < strlen(str); i++ )
    {
        if ( isspace( *(str + i) ) && isspace( *(str + i + 1) ) )
        {
            for ( j = i; j < strlen(str); j++ )
            {
                *(str + j) = *(str + j + 1);
            }
        }
    }
    if ( isspace(*(str + strlen(str) - 1)))
    {
        *(str + strlen(str) - 1) = '\0';
    }
}


