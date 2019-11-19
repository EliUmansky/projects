#include <stdio.h> /* printf */
#define SIZEOF_VAR(x) (long)(&x + 1) - (long)&x

int main ()
{
	int a = 5;

	printf("%ld", SIZEOF_VAR(a));
	


	return 0;
}
