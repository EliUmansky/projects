/*************************
       Unique ID
      Header File
    Author : OL80
      Date : 11/12/2019          
*************************/

#ifndef UNIQUE_ID_H
#define UNIQUE_ID_H

#include <stddef.h> /* size_t */
#include <time.h> /* time_t */
#include <sys/types.h> /* pid_t */

typedef struct
{
	pid_t pid;
	time_t time;
	size_t counter;
} ilrd_uid_t;

extern const ilrd_uid_t g_bad_uid;

/* - Creates a new unique identifier and returns it by value --------------- */
ilrd_uid_t UIDCreate();

/* - returns 1 if both uid_1 and uid_2 are the same, and 0 if not ---------- */
int UIDIsSame(ilrd_uid_t uid_1, ilrd_uid_t uid_2);

/* - returns 1 if uid is bad_uid, and 0 if not ----------------------------- */
int UIDIsBad(ilrd_uid_t uid);

#endif /* UNIQUE_ID_H */


