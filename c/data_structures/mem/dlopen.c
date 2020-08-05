#include <dlfcn.h>
#include <stdio.h>
#include "mem.h"

int main()
{
	void *handle = NULL;
	void(*func_ptr)() = NULL;

	handle = dlopen("/home/student/git/ds/lib/libmem.so", RTLD_NOW);
	func_ptr = dlsym(handle, "foo");

	func_ptr();
	dlclose(handle);

	return 0;
}
