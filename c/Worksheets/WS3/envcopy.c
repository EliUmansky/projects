/********************************************************
         Author: Eli Umansky, Date: 07.11.19
*********************************************************
   This program creates a copy of all the environment 
      variables to lowercase alphabets and clears  
                the memory afterwards
********************************************************/
#include <stdio.h> /*printf*/   
#include <stdlib.h> /*malloc*/
#include <string.h> /*strlen*/
#include <assert.h> /*assert*/
#include <ctype.h> /*tolower*/

#define MALLOC malloc /* malloc / MyMalloc */

void Clear ( char **buffer );
char **Copy ( char *envp[], int len );
void StringCopy ( char *dest, char *source );
char *MyMalloc (size_t size);

int main ( int argc, char *argv[], char **envp )   
{
	char **envrunner = envp;
	char **buffer = NULL;
	char **runbuf = NULL;
	int len = 0;

	while ( NULL != *envrunner )
	{
		++len;
		++envrunner;
	}	

	buffer = Copy ( envp,len );

	if ( NULL == buffer )
	{
		printf ( "NO\n" );
		return -1;
	}   
	
	runbuf = buffer;

	while ( NULL != *runbuf )
	{
		printf("%s\n", *runbuf);
		++runbuf;
	}

	Clear ( buffer );
	( void ) argc;
	( void ) argv;
	
	return 0; 
}

void Clear ( char **buffer ) /*Clears allocated memory*/
{
	char **runbuf = buffer;

	while ( *runbuf != NULL )
	{
		free ( *runbuf );
		++runbuf;
	}
	free ( buffer );
}

char **Copy ( char **envp, int len ) /*Copies environmental variables*/
{
	char **envcpy = NULL;
	char **runner = NULL;
	char **envrunner = envp;
	
	envcpy = ( char** ) calloc ( ( len + 1 ), sizeof ( char* ) );

	if( NULL == envcpy )
	{
		printf( "Not enough space" );
		return NULL;
	}
	
	runner = envcpy;

	while ( NULL != *envrunner )
	{		
		*runner = ( char* ) MALLOC ( strlen ( *envrunner ) + 1 );
		
		if ( NULL == *runner )	
		{
			Clear( envcpy );
			return NULL;
		}		
		
		StringCopy ( *runner, *envrunner );
		++runner;
		++envrunner;		
	}	
	
	return envcpy;
}

void StringCopy ( char *dest, char *source ) /*Copies strings to lowercase*/
{
	char *rundest = dest;
	char *runsource = source;

	assert ( rundest );
	assert ( runsource );

    while ( *runsource )
    {
     	*rundest = tolower ( *runsource );
		++rundest;
		++runsource;
    }
    *rundest = '\0';
}

char *MyMalloc ( size_t size ) /* Stops the allocation after i times*/
{
	static int i = 0;
	
	while ( i < 30 )
	{
		++i;		
		return malloc ( size );	
	}
	
	return NULL;
}
