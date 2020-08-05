#include <stdio.h>


int Strlen (char str1[]);
int Strcmp (char str1[], char str2[]);
void Strncpy (char str3[], char str1[], int num);
void Strcpy (char str3[], char str1[]);
int Strcasecmp (char str1[], char str2[]);
int Strcasecmp2 (char str1[], char str2[]);
char *Strchr (char str1[], char ch);
char *Strdup (char str1[], int Strlen(str1) );
void Strcat (char *str1, char *str2);
void Strncat ( char *str1, char *str2, num );
char *Strstr ( char *str1, char *str2 );
int Strspn ( char *str1, char *str2 );
void Strtok ( char *str1, char *str2, char ch );

int main()
{
    char str1[] = "I am hungry";
    char str2[] = "i Am hungry";
    char str3[20];
    int num = 4;
    char ch = 'h';
    char *string = Strchr(str1, ch);
    printf("%s", string);
    return 0;
}
int Strlen (char str1[])
{
    int len = 0;
    while (str1[len])
    {
        len++;
    }
    return len;
}
int Strcmp (char str1[], char str2[])
{
    int i = 0;
    while ( str1[i] && str2[i] )
    {
        if ( str1[i] != str2[i] )
        {
            return 0;
        }
        i++;
    }
    return ( str1[i] == '\0' && str1[i] == '\0' );

}
void Strncpy (char str3[], char str1[], int num)
{
    int i = 0;
    for ( i = 0; i < num ; i++ )
    {
        str3[i] = str1[i];
    }
    str3[i] = '\0';
}
void Strcpy (char str3[], char str1[])
{
    int i = 0;
    while ( str1[i] )
    {
     str3[i] = str1[i];
     i++;
    }
    str3[i] = '\0';
}
int Strcasecmp (char str1[], char str2[])
{
    int i = 0;
    while ( str1[i] && str2[i] )
    {
        if ( isupper(str1[i]) && isupper(str1[i]) )
        {
                str1[i] = tolower(str1[i]);
                str2[i] = tolower(str2[i]);
                if ( str1[i] != str2[i] )
                {
                    str1[i] = toupper(str1[i]);
                    str2[i] = toupper(str2[i]);
                    return 0;
                }
        }
        else if ( isupper(str1[i]) )
        {
                str1[i] = tolower(str1[i]);
                if ( str1[i] != str2[i] )
                {
                    str1[i] = toupper(str1[i]);
                    return 0;
                }
        }

        else if ( isupper(str2[i]) )
        {
                str2[i] = tolower(str2[i]);
                if ( str1[i] != str2[i] )
                {
                    str2[i] = toupper(str2[i]);
                    return 0;
                }
        }
        else
        {
                if ( str1[i] != str2[i] )
                {
                    return 0;
                }
        }
        i++;
    }
    return ( str1[i] == '\0' && str2[i] == '\0' );
}
int Strcasecmp2 (char str1[], char str2[])
{
    char str1temp[Strlen(str1)+1];
    char str2temp[Strlen(str1)+1];
    Strcpy(str1temp, str1);
    Strcpy(str2temp, str2);
    int i = 0;
        while ( str1temp[i] && str2temp[i] )
        {
            str1temp[i] = tolower( str1temp[i] );
            str2temp[i] = tolower( str2temp[i] );
            if ( str1temp[i] != str2temp[i] )
                {
                    return 0;
                }
            i++;
        }
    return ( str1temp[i] == '\0' && str2temp[i] == '\0' );
}
char *Strchr (char str1[], char ch)
{
    int i = 0;
    while ( str1[i] )
    {
        if ( str1[i] == ch )
        {
            return &str1[i];
        }
        i++;
    }
}
char *Strdup ( char str1[], int Strlen(str1) )
{
    int i = 0;
    char *ptr = (char*)malloc(( Strlen(str1) + 1 ) * sizeof(char));
    for ( i = 0; i < Strlen(str1); i++ )
    {
            *(ptr + i) = *(str1 + i);
    }
    *(ptr + i) = '\0';
    return ptr;
}
void Strcat ( char *str1, char *str2 )
{

    str1 = str1 + Strlen(str1);
    while ( *str2 )
    {
        *str1++ = *str2++;
    }
    *str1 = '\0';
}
void Strncat ( char *str1, char *str2, num )
{
    int i = 0;
    str1 = str1 + Strlen(str1);
    while ( i < num )
    {
        *str1++ = *str2++;
        i++;
    }
    *str1 = '\0';
}
char *Strstr ( char *str1, char *str2 )
{
    int i = 0, j = 0, itemp = 0;
    for ( i = 0; i < Strlen(str1); i++ )
    {
         if ( *(str1 + i) == *str2 )
         {
              itemp = i;
              while ( *(str2 + j) )
              {
                   if ( *(str1 + i) != *(str2 + j) )
                   {
                        i = itemp;
                        j = 0;
                        break;
                   }
                   i++;
                   j++;
              }
              if ( *str2 )
              {
                   return (str1 + itemp);
              }
         }
    }
    return 0;
}
int Strspn ( char *str1, char *str2 )
{
     int i = 0, j = 0, max = 0;
     while ( *(str2 + j) )
     {
          for ( i = 0; *(str1 + i) != '\0'; i++ )
          {
               if ( *(str1 + i) == *(str2 + j) && i > max )
               {
                    max = i;
                    break;
               }
               if ( !*(str1 + i) )
               {
                    return 0;
               }
          }
          j++;
     }
     return ( max + 1 );
}
void Strtok ( char *str1, char *str2, char ch )
{
     int i = 0;
     for ( i = 0; i < strlen(str1); i++ )
     {
          if ( *(str1 + i) == ch )
          {
               *(str2 + i) = '\n';
          }
          else
          {
               *(str2 + i) = *(str1 + i);
          }
     }
     *(str2 + i) = '\0';
}
