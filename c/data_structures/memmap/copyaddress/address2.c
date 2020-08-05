#include <stdio.h>

int main()
{
	int a = 200;
	size_t *address_from_file = NULL;
	FILE *file = NULL;

	file = fopen("file.txt", "r");
	
	fscanf(file, "%lu", address_from_file);

	printf("%d", *(int*)address_from_file);

	fclose(file);

	return 0;
}
