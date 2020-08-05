#include <stdio.h> /* printf */

#define MAX2(a, b) (a > b) ? a : b
#define MAX3(a, b, c) (MAX2(a, b) > c) ? MAX2(a, b) : c

int main ()
{
	int a = 4, b = 6, c = 9;

	printf("%d\n", MAX2(a, b));
	printf("%d\n", MAX3(a, b, c));

	return 0;
}


