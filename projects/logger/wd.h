/*************************
       Watch Dog
      Header File
    Author : OL80
      Date : 17.02.2020
*************************/

#ifndef WATCH_DOG_H
#define WATCH_DOG_H

#include <stddef.h> /* size_t */

/* - starts the watchdog app that guards the calling program and reruns it -- */
/* - if it ever gets terminated. The calling program also "guards" the WD --- */
/* - WD requires sig_user_1 and sig_user_2 to run --------------------------- */
/* - WD requires exclusivity of env variable WATCHDOG_MASTER_PATH ----------- */
/* - WD requires exclusivity of global variable is_watchdog ----------------- */
/* - time complexity O(1) to setup ------------------------------------------ */
int WDStart(char *argv[]);

/* - ends the protection of the WD ------------------------------------------ */
/* - time complexity O(1) --------------------------------------------------- */
int WDEnd(size_t timeout);

#endif /* WATCH_DOG_H */
