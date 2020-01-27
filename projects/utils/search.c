#include <math.h> /* sqrt */

#include "search.h"

#define FOUND (0)
#define NOT_FOUND (1)

static size_t FindJump(size_t nmemb, size_t root) ;

void *BSearch(const void *base, size_t nmemb, size_t size, 
									cmp_func_t cmp_func, const void *key)
{
	size_t memb_left = nmemb;	
	size_t next_jump = memb_left / 2 * size;
	char *searcher = (char*)base + next_jump;
	char *start = (void*)base, *end = (char*)base + (nmemb - 1) * size;
	int cmp_value = NOT_FOUND;

	while (searcher <= end && searcher >= start)	
	{
		cmp_value = cmp_func(key, searcher);
		if (FOUND == cmp_value)
		{
			break;
		}
		memb_left /= 2;
		next_jump = (size == next_jump) ? next_jump : memb_left / 2 * size;
		searcher = (0 < cmp_value) ? searcher + next_jump : searcher - next_jump;
	}

	return (searcher > end || searcher < start) ? NULL : searcher;
}
	
void *RecBSearch(const void *base, size_t nmemb, size_t size, 
									cmp_func_t cmp_func, const void *key)
{
	size_t next_jump = nmemb / 2 * size;
	char *searcher = (char*)base + next_jump;
	int cmp_value = NOT_FOUND;

	if (0 == nmemb)
	{
		return NULL;
	}

	cmp_value = cmp_func(key, searcher);
	if (FOUND == cmp_value)
	{
		return searcher;
	}
	else if (0 < cmp_value)
	{
	
		return RecBSearch(searcher + size, nmemb / 2, size, cmp_func, key);
	}
	else
	{
		return RecBSearch(base, nmemb / 2, size, cmp_func, key);
	}
}

void *JumpSearch(const void *base, size_t nmemb, size_t size, 
									cmp_func_t cmp_func, const void *key)
{
	char *jumper = (void*)base, *end = (char*)base + (nmemb -1) * size;
	size_t root = (size_t)sqrt(nmemb);
	size_t jump = FindJump(nmemb, root);
	int cmp_value = NOT_FOUND;

	jumper += jump * size;

	while (jumper <= end && 0 < (cmp_value = cmp_func(key, jumper)))
	{
		jumper += jump * size;
		--jump;
	}

	if (FOUND == cmp_value)
	{
		return jumper;
	}
	else if (jumper > end)
	{
		jumper = end;
	}

	while (0 < jump && 0 > cmp_func(key, jumper))
	{
		jumper -= size;
		--jump;
	}

	return (0 == cmp_func(key, jumper)) ? jumper : NULL;
}

static size_t FindJump(size_t nmemb, size_t root) 
{ 
	size_t jump = root, arr_len = jump * (jump + 1) / 2; 

    if (0 == nmemb || 1 == nmemb)
	{ 
    	return 0; 
	}

    while (arr_len < nmemb) 
    { 
      ++jump; 
      arr_len = jump * (jump + 1) / 2; 
    }
 
    return jump; 
} 
