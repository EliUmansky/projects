#include <stdio.h> /* printf */
#define SIZEOF_TYPE(x) ((x*)0 + 1)

int main ()
{
	printf("%ld", SIZEOF_TYPE(int));

	return 0;
}
