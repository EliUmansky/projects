#include <stdio.h>

int main()
{
	int a = 100;
	FILE *file = NULL;
	char str[20] = " ";

	sprintf(str, "%lu", (size_t)&a);
	file = fopen("file.txt", "w");
	fputs(str, file);

	fclose(file);

	return 0;
}
