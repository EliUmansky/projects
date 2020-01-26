/***********************************************************************/
/*              Author: Eli Umansky   Date 21.11.19                    */
/***********************************************************************/
/*    This file contains all the functions of the header BitsArray.h   */
/***********************************************************************/

#include "bitarray.h"
#include <stdio.h>
#include <assert.h>

#define LEFT_BIT_ON 0x8000000000000000
#define SIZEBARR sizeof(barr_t)
#define BITSINBARR (sizeof(barr_t) * 8)

barr_t BarrSetBit(barr_t arr, barr_t index, int value)
{
	assert (63 >= index);
	assert ((0 == value) || (1 == value));

	return (0 == value) ? (arr ^ (0x1LU << index)) : (arr | 0x1LU << index);
}

barr_t BarrSetOn(barr_t arr, barr_t index)
{
	assert (63 >= index);

	return (arr | 0x1LU << index);
}

barr_t BarrSetOff(barr_t arr, barr_t index)
{
	assert (63 >= index);

	return (arr & ~(0x1LU << index));	
}

barr_t BarrFlipBit(barr_t arr, barr_t index)
{
	assert (63 >= index);

	return (arr ^ 0x1LU << index);
}

int BarrIsOn(barr_t arr, barr_t index)
{
	assert (63 >= index);

	return ((arr & (0x1LU << index)) == arr);
}

int BarrIsOff(barr_t arr, barr_t index)
{
	assert (63 >= index);

	return ((arr | (0x1LU << index)) != arr);
}

size_t BarrCountOn(barr_t arr)
{
	barr_t arr_copy = arr;
	size_t count = 0;

	while (0 != arr_copy)
	{
		if ((arr_copy | 0x1LU) == arr_copy)
		{
			++count;
		}

		arr_copy >>= 1;
	}

	return count;
}
size_t BarrCountOnLut(barr_t arr)
{
	barr_t arr_copy = arr;	
	static unsigned char barr_lut[256] = {0};
	unsigned char mask  = 0xff;
	unsigned int i = 0; 
	size_t count = 0;

	if (0 == barr_lut[1])
	{
		for (; i < 256; ++i)
		{
			barr_lut[i] = (i & 0x55555555) + ((i >> 1) & 0x55555555);
			barr_lut[i] = (barr_lut[i] & 0x33333333) + 
               			  ((barr_lut[i] >> 2) & 0x33333333);
			barr_lut[i] = (barr_lut[i] & 0x0f0f0f0f) +
                  		  ((barr_lut[i] >> 4) & 0x0f0f0f0f);
		}
	}
	
	for (i = 0; i < (SIZEBARR); ++i)
	{
		count += (size_t)(barr_lut[arr_copy & mask]);
		arr_copy >>= SIZEBARR;
	}
	
	return count; 
}

size_t BarrCountOff(barr_t arr)
{
	return (BITSINBARR - BarrCountOn(arr));
}

barr_t BarrSetAll(barr_t arr)
{
	return (arr | (~0x0));
}

barr_t BarrResetAll(barr_t arr)
{
	return (arr & 0x0);
}

barr_t BarrRotL(barr_t arr, int times_rot)
{
	int rot_remainder = times_rot % BITSINBARR;

	return ((arr << rot_remainder) | (arr >> (BITSINBARR - rot_remainder)));
}

barr_t BarrRotR(barr_t arr, int times_rot)
{
	int rot_remainder = times_rot % (SIZEBARR * 8);

	return ((arr >> rot_remainder) | (arr << (BITSINBARR - rot_remainder)));

}

barr_t BarrMirror(barr_t arr)
{
	arr = (arr << 32 | arr >> 32);
	arr = (((arr << 16) & 0xffff0000ffff0000) | 
		  ((arr >> 16) & 0x0000ffff0000ffff));
	arr = (((arr << 8) & 0xff00ff00ff00ff00) | 
		  ((arr >> 8) & 0x00ff00ff00ff00ff));
	arr = (((arr << 4) & 0xf0f0f0f0f0f0f0f0) | 
		  ((arr >> 4) & 0x0f0f0f0f0f0f0f0f));
	arr = (((arr << 2) & 0xcccccccccccccccc) | 
		  ((arr >> 2) & 0x3333333333333333));
	arr = (((arr << 1) & 0xaaaaaaaaaaaaaaaa) | 
		  ((arr >> 1) & 0x5555555555555555));

	return arr;
}

barr_t BarrMirrorLut(barr_t arr)
{
	static unsigned char barr_lut[256] = {0};
	barr_t arr_copy = 0x0;
	unsigned char mask  = 0xff;
	unsigned int i = 0; 

	if (0 == barr_lut[1])
	{
		for (; i < 256; ++i)
		{
			barr_lut[i] = (i << 4 | i >> 4);
			barr_lut[i] = (((barr_lut[i] << 2) & 0xcc) |
        	               ((barr_lut[i] >> 2) & 0x33));
			barr_lut[i] = (((barr_lut[i] << 1) & 0xaa) | 
        	               ((barr_lut[i] >> 1) & 0x55));
		}
	}

	for (i = 0; i < (SIZEBARR); ++i)
	{
		if (0 != arr_copy)
		{
			arr_copy <<= SIZEBARR;		
		}	

		arr_copy |= (barr_t)barr_lut[arr & mask];
		arr >>= SIZEBARR;
	}

	return arr_copy;
}

char *BarrToString(barr_t arr, char* str)
{
	barr_t arr_copy = arr;
	char *runner = str;
	int bit_count = BITSINBARR;

	for (; 0 < bit_count; --bit_count)
	{
		if ((LEFT_BIT_ON & arr_copy) == LEFT_BIT_ON)
		{
 			*runner = '1';
		}

		else
		{
			*runner = '0';
		}

		arr_copy <<= 1;
		++runner;
	}

	*runner = '\0';

	return str;
}








