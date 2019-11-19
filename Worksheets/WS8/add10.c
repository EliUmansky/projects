/*********************************************/
/* AUTHOR: Eli Umansky ----- DATE: 18.11.19  */
/*********************************************/
/* This program creates an array of elements */
/*   of different types, add 10 to each of   */
/*   them and prints the different results   */
/*********************************************/

#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*strcpy*/

typedef struct{
	void *data;
	void (*Printer)(void *data);
	void (*Add)(int num, void **data);
} element_t;

element_t array[3] = {0};
int num = 10;	

void PrintInt (void *data);
void PrintFloat (void *data);
void PrintStr (void *data);
void AddInt (int num, void **data);
void AddFloat (int num, void **data);
void AddStr (int num, void **data);
void PrintArray ();
void AddToArray ();

int main ()
{
	char *string = (char*)malloc(50);
	
	if (NULL == string)
	{
		printf("Allocation error");		
	
		return 1;
	}

	strcpy(string, "chapter");

	*(int*)&array[0].data =  5;
	array[0].Printer = PrintInt;
	array[0].Add = AddInt;
	*(float*)&array[1].data = 6.25;
	array[1].Printer = PrintFloat;
	array[1].Add = AddFloat;
	array[2].data = string;
	array[2].Printer = PrintStr;
	array[2].Add = AddStr;

	PrintArray();
	AddToArray();
	PrintArray();
	
	free(string);

	return 0;
}

void PrintArray ()
{
	int i = 0;	
	
	for (; i <= 2; ++i)
	{
		array[i].Printer (array[i].data);
	}
}

void AddToArray ()
{
	int i = 0;	

	for (; i <= 2; ++i)
	{
		array[i].Add (num, &array[i].data);
	}
}

void PrintInt (void *data)
{
	printf("%d ", *(int*)&data);
}

void PrintFloat (void *data)
{
	printf("%f ", *(float*)&data);
}
void PrintStr (void *data)
{
	printf("%s ", (char*)data);
}
void AddInt (int num, void **data)
{
	*(int*)data = *(int*)data + num;
}
void AddFloat (int num, void **data)
{
	*(float*)data =  *(float*)data + (float)num;
}
void AddStr (int num, void **data)
{
	char *str = (char*)malloc(3);
	if (NULL == str)
	{
		printf("Allocation error");		
	}

	sprintf(str, "%d", num);	
	strcat(*data, str);
	
	free(str);
	
}
