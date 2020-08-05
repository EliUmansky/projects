#include "uid.h"

size_t counter;

size_t Count();

const ilrd_uid_t g_bad_uid = {0};

ilrd_uid_t UIDCreate()
{
	ilrd_uid_t uid = {0};

	uid.pid = getpid();
	uid.time = time(NULL);
	uid.counter = Count(counter);

	return uid;
}

int UIDIsSame(ilrd_uid_t uid_1, ilrd_uid_t uid_2)
{
	return ((uid_1.pid == uid_2.pid) &&
			(uid_1.time == uid_2.time) &&
			(uid_1.counter == uid_2.counter));
}

int UIDIsBad(ilrd_uid_t uid)
{
	return (UIDIsSame(uid, g_bad_uid));
}

size_t Count()
{
	++counter;

	return counter;
}
