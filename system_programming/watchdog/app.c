#include <unistd.h> /* sleep */
#include <stdlib.h> /* setenv */
#include "wd.h"

extern int is_watchdog;

int main(int argc, char **argv)
{
	int i = 0;

	(void)argc;
	is_watchdog = 0;
	WDStart(argv);

	for (; i < 160; ++i)
	{
		sleep(1);
	}

	WDEnd(5);

	return 0;
}
