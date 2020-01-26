#include <stdio.h> /* size_t */
#include <stdlib.h> /*malloc */
#include "fsa.h" /* fsa_t */

static void test(int expected, int actual, const char* testName);

void TestFlow1();
void TestFlow2();
void TestFlow3();
void TestFlow4();
void TestFlow5();
void TestFlow6();
void TestFlow7();

struct fsa
{
	size_t block_size;
	size_t num_of_blocks;
	size_t next_free;
};

typedef struct
{
	size_t header_offset;
} block_header_t;

int main()
{
	TestFlow1();
	TestFlow2();
	TestFlow3();
	TestFlow4();
	
	return 0;
}

void TestFlow1()
{
	size_t block_size_1 = 8;
	size_t block_size_2 = 4;
	size_t num_of_blocks = 6;
	size_t suggested_size_1 = 0;
	size_t suggested_size_2 = 0;

	puts("Flow 1: SuggestedSize Function");
	puts("------------------------------");
	suggested_size_1 = FSASuggestedSize(block_size_1, num_of_blocks);
	suggested_size_2 = FSASuggestedSize(block_size_2, num_of_blocks);
	
	test(120, suggested_size_1, "|| Suggested size");
	test(120, suggested_size_2, "|| Suggested size when small size is requested\n");

}

void TestFlow2()
{
	int *pool = NULL;	
	fsa_t *fsa = NULL;
	size_t block_size = 8;
	size_t num_of_blocks = 4;
	size_t suggested_size = 0;
	

	puts("Flow 2: Init, CountFree");
	puts("-----------------------");
	suggested_size = FSASuggestedSize(block_size, num_of_blocks);

	pool = (int*)malloc(suggested_size);
	fsa = FSAInit(pool, block_size, num_of_blocks);

	test(88, suggested_size, "|| Checking size just to be sure");

	test(32, fsa->next_free, "|| Value of next->free");
	
	test(4, FSACountFree(fsa), "|| Count free = 4");
	test(24, ((block_header_t*)((char*)fsa + 24))->header_offset, "|| First b_header value");
	test(40, ((block_header_t*)((char*)fsa + 40))->header_offset, "|| Second b_header value");
	test(48, *(size_t*)((char*)fsa + 32), "|| First block next value");
	test(64, *(size_t*)((char*)fsa + 48), "|| Second block next value");
	test(0, *(size_t*)((char*)fsa + 80), "|| Last block next value = 0\n");
}

void TestFlow3()
{
	int *pool = NULL;	
	fsa_t *fsa = NULL;
	size_t *alloc_address = NULL;	
	size_t block_size = 8;
	size_t num_of_blocks = 4;
	size_t suggested_size = 0;
	

	puts("Flow 3: Alloc");
	puts("-----------------------");
	suggested_size = FSASuggestedSize(block_size, num_of_blocks);

	pool = (int*)malloc(suggested_size);
	fsa = FSAInit(pool, block_size, num_of_blocks);

	test(32, fsa->next_free, "|| next_free value");
	alloc_address = FSAAlloc(fsa);
	test(48, *alloc_address, "|| Is allocation address correct");
	test(48, fsa->next_free, "|| Is new next_free correct");
	test(3, FSACountFree(fsa), "|| Count free = 3");

	FSAAlloc(fsa);
	test(2, FSACountFree(fsa), "|| Count free = 2");	
	FSAAlloc(fsa);
	test(1, FSACountFree(fsa), "|| Count free = 1");
	FSAAlloc(fsa);
	test(0, FSACountFree(fsa), "|| Count free = 0\n");
}

void TestFlow4()
{
	int *pool = NULL;	
	fsa_t *fsa = NULL;
	size_t *alloc_address_1 = NULL;	
	size_t *alloc_address_2 = NULL;	
	size_t *alloc_address_3 = NULL;	
	size_t *alloc_address_4 = NULL;	
	size_t block_size = 8;
	size_t num_of_blocks = 4;
	size_t suggested_size = 0;
	

	puts("Flow 3: Free");
	puts("------------");
	suggested_size = FSASuggestedSize(block_size, num_of_blocks);

	pool = (int*)malloc(suggested_size);
	fsa = FSAInit(pool, block_size, num_of_blocks);
	
	alloc_address_4 = FSAAlloc(fsa);
	alloc_address_2 = FSAAlloc(fsa);
	alloc_address_3 = FSAAlloc(fsa);
	alloc_address_1 = FSAAlloc(fsa);

	test(0, FSACountFree(fsa), "|| Count free (after allocations) = 0");
	FSAFree(NULL);
	test(0, FSACountFree(fsa), "|| Count free (after freeing NULL) = 0");

	FSAFree(alloc_address_1);
	test(1, FSACountFree(fsa), "|| Count free (after free) = 1");
	FSAFree(alloc_address_3);
	test(2, FSACountFree(fsa), "|| Count free (after free) = 2");
	FSAFree(alloc_address_4);
	test(3, FSACountFree(fsa), "|| Count free (after free) = 3");
	FSAFree(alloc_address_2);
	test(4, FSACountFree(fsa), "|| Count free (after free) = 4");
}
static void test(int expected, int actual, const char* testName)
{
	if (expected == actual)
	{
	    printf("PASSED %s\n", testName);
	}
    else
    {
		printf("FAILED %s \n", testName);
		printf("   expected: %d actual: %d\n", expected, actual);
    }
}

