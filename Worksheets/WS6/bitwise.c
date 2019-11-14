/********************************************************
          Author: Eli Umansky, Date: 13.11.19
*********************************************************
            Worksheet - Bitwise Operations
********************************************************/
#include <stdio.h> /* printf */

long Pow2 (unsigned int x, unsigned int y) /* returns the resut of x*(2^y) */
{
	long result = x << y;

	return result;
}

int IfPow2 (unsigned int n) /* returns 1 if n is a power of 2, otherwise - 0*/
{
    return ((0 == (n & (n - 1))) && 0 != n);
}

int IfPow2Loop (unsigned int n)  /* like IfPow2, but without a loop */
{
	while (1 < n && 0 == n % 2)
	{
		n >>= 1;
	}
	
	return (1 == n) ? 1 : 0;	
}

void SwapTwoVars (int *var1, int *var2) /* swaps two variables using XOR */
{
	if (var1 != var2)
	{
    	*var1 ^= *var2;
    	*var2 ^= *var1;
    	*var1 ^= *var2;
	}
}

void Add1 (int *a) /* adds 1 to an int using bitwise operations */
{
    int temp = 1;
	
    while ((*a == (*a | temp)) && -2147483648 != temp)
    {
        temp <<= 1;
    }
    
	while (0 < temp)
	{
        *a ^= temp;
        temp >>= 1;
    }

	if (-1 == *a)
	{
		*a = *a & 0;
	}	
}

void Print3Bits (unsigned int *arr, size_t arr_len) /* gets an array and */
{                                                   /* prints the numbers that*/
    unsigned int temp = 0;                          /* have exactly 3 bits on */
    unsigned int *runner = arr;
    int count = 0;

    while (runner < arr + arr_len)
    {
    	for (temp = *runner, count = 0; 0 < temp; temp >>= 1)   
		{
            if (0 != temp % 2)
            {
                ++count;
            }
        }
      
	    if (3 == count)
        {
            printf("%d ", *runner);
        }
      
		++runner;
    }
}

unsigned char ByteMirrorLoop (unsigned char num) /* returns a new number with */
{                                                /*   reverse order of bits  */
    unsigned char run_end = 1;
    unsigned char run_begin = 128;
    unsigned char sum = 0;

    while (1 <= run_begin)
    {
        if ((num|run_begin) == num)
        {
            sum += run_end;
        }

        run_end <<= 1;
        run_begin >>= 1;
    }

    return sum;
}

int BitTwoAndSix (unsigned char ch) /* checks if bits 2 and 6 are on  */
{
    return ((ch == (ch | 2)) && (ch == (ch | 32)));
}

int BitTwoOrSix (unsigned char ch)
{
    return ((ch == (ch | 2)) || (ch == (ch | 32)));
}

unsigned int Divisible16 (unsigned int num) /* checks if bits 2 or 6 are on  */
{
    return (num >> 4) << 4;
}

int CountBitsLoop (int num) /* counts the number of set bits in an integer */
{
    int count = 0;

    while (0 != num)
    {
        count += num % 2;
        num >>= 1;
    }

    return count;
}

unsigned char Swap3And5 (unsigned char ch) /* swaps between bit 3 and bit 5*/
{
    unsigned char result = 0, temp = 0;

    temp = ((ch >> 2) ^ (ch >> 4)) & 1;
    result = ch ^ ((temp << 2) | (temp << 4));

    return result;
}

unsigned char ByteMirror(unsigned char ch) /* returns a new number with a */
{                                          /*    reverse order of bits    */
	ch = (ch << 4 | ch >> 4);
	ch = (((ch << 2) & 204) | ((ch >> 2) & 51));
	ch = (((ch << 1) & 170) | ((ch >> 1) & 85));

	return ch;	
}

unsigned int CountBits (unsigned int num) /* counts the number of set */
{                                         /*    bits in an integer    */  
	num = (num & 0x55555555) + ((num >> 1) & 0x55555555);
	num = (num & 0x33333333) + ((num >> 2) & 0x33333333);
	num = (num & 0x0f0f0f0f) + ((num >> 4) & 0x0f0f0f0f);
	num = (num & 0x00ff00ff) + ((num >> 8) & 0x00ff00ff);
	num = (num & 0x0000ffff) + ((num >> 16) & 0x0000ffff);

	return num;
}

void FloatToBinary () 
{
	float f_num = 67.4;
	int *i_num = (int*)&f_num;
	unsigned int temp = 0x80000000;
	int ZeroOrOne = 0;
	
	while (0 < temp)
	{
		ZeroOrOne = *i_num & temp;	
	
		if (0 == ZeroOrOne)		
		{
			printf("0");
		}
		else
		{
			printf("1");
		}

		temp >>= 1;
	}

}

