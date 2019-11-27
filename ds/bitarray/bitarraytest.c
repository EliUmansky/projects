#include <stdio.h>
#include "BitsArray.h"

#define RUN_TEST(test, exp) (test == exp) ? printf("OK\n") : printf("FAIL\n"); 

int main()
{
	barr_t arr = 0xf0200a008000008d;
	char str[65] = "Hello world";

	barr_t arr2 = 0x0;
	
	printf("%s\n", BarrToString(arr, str));
	printf("%s\n", BarrToString(BarrMirrorLut(arr), str));
	printf("%lu\n", BarrCountOnLut(arr));

	printf("%s\n", BarrToString(arr2, str));
	printf("%s\n", BarrToString(BarrMirrorLut(arr2), str));
	printf("%lu\n", BarrCountOnLut(arr2));

	/*RUN_TEST(BarrSetBit(arr, 7, 1), 0xf0200a008000008d);
	RUN_TEST(BarrSetBit(arr, 2, 1), 0x0000000000000004);
	arr = BarrSetBit(arr, 2, 1);
	RUN_TEST(BarrSetBit(arr, 4, 1), 0x0000000000000014);
	arr = BarrSetBit(arr, 4, 1);
	RUN_TEST(BarrSetBit(arr, 4, 0), 0x0000000000000004);
	arr = BarrSetBit(arr, 4, 0);
	printf("%s", BarrToString(arr, str));
	RUN_TEST(BarrSetOn(arr, 4), 0x0000000000000014);
	arr = BarrSetOn(arr, 4);
	RUN_TEST(BarrSetOff(arr, 4), 0x0000000000000004);
	arr = BarrSetOff(arr, 4);
	RUN_TEST(BarrFlipBit(arr, 4), 0x0000000000000014);
	BarrFlipBit(arr, 4);
	RUN_TEST(BarrIsOn(arr, 2), 1);
	RUN_TEST(BarrIsOn(arr, 16), 0);

	RUN_TEST(BarrIsOff(arr, 2), 0);
	RUN_TEST(BarrIsOff(arr, 16), 1);

	arr = BarrSetOn(arr, 16);
	arr = BarrSetOn(arr, 23);

	RUN_TEST(BarrCountOn(arr), 3);
		
	RUN_TEST(BarrCountOff(arr), 61);

	RUN_TEST(BarrSetAll(arr), 0xffffffffffffffff);

	RUN_TEST(BarrResetAll(arr), 0x0);
	arr = BarrResetAll(arr);
	arr = BarrSetOn(arr, 0);
	arr = BarrSetOn(arr, 1);
	arr = BarrSetOn(arr, 2);
	arr = BarrSetOn(arr, 3);

	RUN_TEST(BarrRotL(arr, 66), 0x000000000000003c);

	arr = BarrRotL(arr, 66);
printf("%s\n", BarrToString(arr, str));
	RUN_TEST(BarrRotR(arr, 66), 0x000000000000000f);
arr = BarrRotR(arr, 2);
printf("%s\n", BarrToString(arr, str));

arr = BarrMirror(arr);
printf("%s\n", BarrToString(arr, str));*/
	return 0;
}

