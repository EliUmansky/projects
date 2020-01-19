#include <stdio.h>
#include <unistd.h>
#include "mem.h"

const int const_glob_int = 3;
int glob_int = 4;

void foo()
{
	int loc_int = 5;
	static int stat_loc_int = 1;
	char *str = "I'm tired";

	++stat_loc_int;
	glob_int += const_glob_int;

	printf("%d", stat_loc_int);
	printf("%d", loc_int);
	printf("%d", const_glob_int);
	printf("%d", glob_int);
	printf("%s", str);

	sleep(2);
}

