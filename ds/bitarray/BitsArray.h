/***********************/
/*		BitsArray.h	   */
/*	 Date: 21.11.2019  */
/*		   OL80		   */
/***********************/

#ifndef BARR_H
#define BARR_H

#include <stdio.h>

typedef size_t barr_t;

/*Returns arr with value inserted to index, value = 0 or value = 1, 0 <= index < 64*/
barr_t BarrSetBit(barr_t arr, barr_t index, int value);
/*Returns arr with bit in place index turned ON, 0 <= index < 64*/
barr_t BarrSetOn(barr_t arr, barr_t index);
/*Returns arr with bit in place index turned OFF, 0 <= index < 64*/
barr_t BarrSetOff(barr_t arr, barr_t index);
/*Returns arr with bit in place index turned to other mode, 0 <= index < 64*/
barr_t BarrFlipBit(barr_t arr, barr_t index);
/*Returns 1 if bit in place index of array is ON, 0 <= index < 64*/
int BarrIsOn(barr_t arr, barr_t index);
/*Returns 1 if bit in place index of array is OFF, 0 <= index < 64*/
int BarrIsOff(barr_t arr, barr_t index);
/*Returns 1 if bit in place index of array is ON, 0 <= index < 64*/
size_t BarrCountOn(barr_t arr);
/*Returns 1 if bit in place index of array is ON using lut, 0 <= index < 64*/
size_t BarrCountOnLut(barr_t arr);
/*Returns 1 if bit in place index of array is OFF, 0 <= index < 64*/
size_t BarrCountOff(barr_t arr);
/*Returns arr with all bits turned ON*/
barr_t BarrSetAll(barr_t arr);
/*Returns arr with all bits turned OFF*/
barr_t BarrResetAll(barr_t arr);
/*Returns arr rotated times_rot to the RIGHT*/
barr_t BarrRotR(barr_t arr, int times_rot);
/*Returns arr rotated times_rot to the LEFT*/
barr_t BarrRotL(barr_t arr, int times_rot);
/*Returns arr with bits mirrored*/
barr_t BarrMirror(barr_t arr);
/*Returns string of arr, string must not be NULL, string must be at least size 65, */
char *BarrToString(barr_t arr, char* str);
/*Returns arr with bits mirrored using lut*/
barr_t BarrMirrorLut(barr_t arr);

#endif /* BARR_H */

