/********************************************************
         Author: Eli Umansky, Date: 11.11.19
*********************************************************
         This program modifies a chosen file, 
            according to the user's input
********************************************************/
#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*strcmp*/

typedef enum { success, user_exit, mem_fail, file_error,
			   remove_error } return_value_t;	

typedef struct
{
	char *string;
	int (*Compare)(const char *string, const char *str);
	int (*OperFunc)(char *string, const char *filename);
} logger_t;

int Logger (const char *filename);
int AddLineEnd (char *str, const char *filename);
int AddLineBegin (char *str, const char *filename);
int RemFile (char *str, const char *filename);
int ProgExit (char *str, const char *filename);
int NumOfLines (char *str, const char *filename);
int OneCharCmp (const char *string, const char *str);
int AlwTrueCmp (const char *string, const char *str);

logger_t g_chain[] = {
	{ "-exit\n", strcmp, ProgExit },
	{ "-count\n", strcmp, NumOfLines },
	{ "-rem\n", strcmp, RemFile },
	{ "<", OneCharCmp, AddLineBegin },
	{ " ", AlwTrueCmp, AddLineEnd } };

int main (int argc, char *argv[])
{		
	return_value_t ret_val = success;	
	
	if ( 2 == argc )
	{
		ret_val = Logger (argv[1]);
	}
	else 
	{
		printf ("Wrong number of arguments");
		return file_error;
	}
	
	return ret_val;
}

int Logger (const char *filename)
{
	char *buf = NULL;
	logger_t *run_chain = g_chain;
	return_value_t ret_val = success;
	int comp_val = 1;
	
	buf = (char *)malloc(50);

	if (NULL == buf)
	{
		return mem_fail;
	}

	while (success == ret_val)
	{
		comp_val = 1;    	
		run_chain = g_chain;
    	fgets (buf, sizeof (buf), stdin);
    	while (0 != comp_val)
    	{
			comp_val = run_chain->Compare (run_chain->string, buf); 

    	    if (0 == comp_val) 
    	    {
    	    	ret_val = run_chain->OperFunc (buf, filename);
    	    }
    	    ++run_chain;
    	}    
	}

	free (buf);

	return ret_val;
}

int AddLineEnd (char *str, const char *filename) /* Adds the input at the end */ 
{                                                /*       of the file         */
	FILE *fp = fopen (filename,"a");

	if (NULL == fp)
	{
		return file_error;
	}

	fputs (str, fp);
	fclose (fp);

	return success;
}

int RemFile (char *str, const char *filename) /* Removes the file */
{
	remove (filename);
	
	return remove (filename) ? success : remove_error;
}

int ProgExit (char *str, const char *filename) /* Exits the program*/
{
	return user_exit;
}

int NumOfLines (char *str, const char *filename) /* Counts the number of lines*/
{					                             /*        in the file        */
	FILE *fp = fopen (filename,"r");

	if (NULL == fp)
	{
		return file_error;
	}

	char c = '\0';
	int count = 0;

	for (c = fgetc (fp); c != EOF; c = fgetc (fp))
    {
        if ('\n' == c)
        {
            ++count;
        }
    }
	
	printf ("%d\n", count);
	fclose (fp);

	return success;
}

int AddLineBegin (char *str, const char *filename) /*  Adds the input at the  */ 
{                                                  /*  beginning of the file  */
	char c = '\0';
	FILE *fp = fopen (filename,"r+");
	FILE *fp2 = fopen ("copytext.txt", "a+");

	if (NULL == fp)
	{
		return file_error;
	}
	
	if (NULL == fp2)
	{
		return file_error;
	}

	while ((c = fgetc (fp)) != EOF) 
	{
	    fputc (c, fp2); 
	}
	
	fclose (fp);
	fclose (fp2);
	fp = fopen (filename, "r+");
	fputs (str + 1, fp);
	fp2 = fopen ("copytext.txt", "r");
	
	while ((c = fgetc (fp2)) != EOF) 
	{
	    fputc (c, fp); 
	}

	remove ("copytext.txt");
	fclose (fp);

	return success;
}
int OneCharCmp (const char *string, const char *str) /*  Compares the first   */
{                                                    /*  char of the strings  */
	return (*str == *string) ? 0 : 1;
}
int AlwTrueCmp (const char *string, const char *str) /*    Always returns 0   */
{
	return 0;
}

