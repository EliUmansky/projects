#include <stdlib.h> /* getenv */

#include "wd.h"

extern int is_watchdog;

int main(int argc, char **argv)
{
	(void)argc;

	is_watchdog = 1;
	WDStart(argv);

	return 0;
}
