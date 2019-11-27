/********************************************************/
/*         Author: Eli Umansky   Date 20.11.19          */
/********************************************************/
/*  This function implements library function memset()  */

#define WORD 8

void *MemSet(void *str, int copyval, size_t num)
{
	char *runner1 = str;
	char *runner8 = NULL;
	unsigned long longval = 0;

	while (0 == (longval >> 56))
	{
		longval <<= 8;
		longval |= copyval;
	} 

	for (; (0 != (unsigned long) runner1 % WORD) && (0 != num); ++runner1, --num)
	{		
		*runner1 = (unsigned char) copyval;
	}

	for (runner8 = runner1; num >= WORD; runner8 += WORD, num -= WORD)
	{
		*(unsigned long*) (runner8) = longval;
	}

	for (runner1 = runner8; 0 < num; ++runner1, --num)
	{
		*runner1 = (unsigned char) copyval;
	}

	return str;
}

/*  This function implements library function memcpy()  */

void *MemCpy(void *dest, const void *source, size_t num)
{
	char *run_dest = dest;
	const char *run_source = source;

	for (; (0 != (unsigned long) run_dest % WORD) && (0 != num); --num)
	{
		*run_dest = *run_source;
		++run_dest;
		++run_source;
	}

	for (; num >= WORD; run_dest += WORD, run_source += WORD, num -= WORD)
	{
		*(unsigned long*) (run_dest) = *(unsigned long*) (run_source);
	}
	
	for (; 0 < num; ++run_dest, ++run_source, --num)
	{
		*run_dest = *run_source;
	}

	return dest;
}
/*  This function implements library function memmove()  */

void *MemMove(void *dest, const void *source, size_t num)
{
	const char *run_source = (const char*) source + num;
	char *run_dest =  (char*) dest + num;
	
	if (source > dest)
	{
		return MemCpy(dest, source, num);
	}

	for (; (0 != (unsigned long) run_dest % WORD) && (0 < num); --num)
	{
		--run_dest;
		--run_source;
		*run_dest = *run_source;
	}

	for (; num >= WORD; num -= WORD)
	{
		run_dest -= WORD;
		run_source -= WORD;
		*(unsigned long*) (run_dest) = *(unsigned long*) (run_source);
	}

	for (; 0 < num; --num)
	{		
		--run_dest;
		--run_source;
		*run_dest = *run_source;
	}

	return dest;
}
/*  This function implements library function itoa() for base 10  */

char *IToA(int num, char *str, int base)
{
	int temp_num = num;
	int tens = 1;
	char *runner = str;

	while (0 != (temp_num / 10))
	{
		tens *= 10;
		temp_num /= 10;
	}	
	
	temp_num = num;

	if ( 0 > temp_num)
	{
		*runner = '-';
		++runner;
		temp_num = -temp_num;
	}

	while (0 != temp_num)
	{ 
		*runner = (temp_num / tens) + 48;
		temp_num %= tens;
		tens /= 10;
		++runner;
	}		

	*runner = '\0';

	return str;
}
/*  This function implements library function atoi() for base 10  */

int AToI(const char *str)
{
	int sum = 0, tens = 1;
	const char *runner = str;

	while ('\0' != *runner)
	{
		++runner;
	}	

	--runner;

	while (runner > str)
	{
		if ('-' == *runner)
		{
			sum = -sum;
			--runner;
		}		
		else
		{
			sum += (*runner - 48) * tens;
			tens *= 10;
			--runner;		
		}
	}

	return sum;
}
/*  This function implements library function itoa() for all bases  */

char *AtoiAllBase(int num, char *str, int base)
{
	int temp_num = num;
	int remainder = 0;
	char *runner = str;

	while (0 != temp_num)
	{
		remainder = temp_num % base;

		if (remainder < 9)
		{
			*runner = remainder + '0';
			++runner;
		}
		
		else
		{
			*runner = 'a' + (remainder - 10);
			++runner;
		}

		temp_num /= base;
	}		
	
	if (0 > num)
	{
		*runner = '-';
		++runner;
	}	

	*runner = '\0';

	RevStr(str);

	return str;
}

void RevStr(char *str)
{
	char *run_start = str;
	char *run_end = str + strlen(str) -1;
	char storechar = '\0';

	while (run_start < run_end)
	{
		storechar = *run_end;
		*run_end = *run_start;
		*run_start = storechar;
		++run_start;
		--run_end;
	}

}
/*  This function implements library function atoi() for all bases  */

int AtoiAllBase(const char *str, int base)
{
	int num = 0, pow = 1;
	const char *runner = str;

	while ('\0' != *runner)
    	{
    	    ++runner;
    	}
    	--runner;

	while (runner >= str)
	{
		if (Value(runner) >= base)
        	{
        	    printf("Invalid number");
        	    return -1;
        	}

        	num += Value(runner) * pow;
        	pow *= base;
        	--runner;
	}

	return num;
}

int Value(const char *runner)
{
    if ('0' <= *runner && '9' >= *runner)
    {
        return ((int)*runner - '0');
    }
    else
    {
        return ((int)*runner - 'a' + 10);
    }
}
/*   This function prints all characters that   */
/*    appear in str1 + str2 but not in str3     */

void CharAppear(char *str1, char *str2, char *str3)
{
	char *lut_ascii = (char*)calloc(256, 1);
	char *runner1 = str1, *runner2 = str2, *runner3 = str3; 

	while ('\0' != *runner1)
	{
		if (0 == lut_ascii[*runner1])
		{
			++lut_ascii[*runner1];
		}
		
		++runner1;
	}

	while ('\0' != *runner3)
	{
		if (1 == lut_ascii[*runner3])
		{
			--lut_ascii[*runner3];
		}
		
		++runner3;
	}

	while ('\0' != *runner2)
	{
		if (1 == lut_ascii[*runner2])
		{
			printf("%c", *runner2);
			++lut_ascii[*runner2];
		}
		
		++runner2;
	}
}
/* This function prints the system endianess*/

void IsBigEndian()
{
	int num = 1;
	int *pnum = &num;
	char *charpnum = (char*)pnum;

	if (1 == *charpnum)
	{
		printf("Litle Endian\n");
	}
	else
	{
		printf("Big Endian\n");
	}
}
