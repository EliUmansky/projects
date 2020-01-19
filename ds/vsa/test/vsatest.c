#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <sys/types.h> /* ssize_t */

#include "vsa.h" /* vsa_t */

typedef struct blk_header
{
	#ifndef NDEBUG
	size_t magic_number;
	#endif
	size_t b_size;
} blk_header_t;

static void test(int expected, int actual, const char* testName);
void PrintEverything (vsa_t *vsa);

void InitAndLargestBlock();
void AllocAndLargestBlock();
void AllocAndFree();
void AllocAndFreeAdvanced();

int main()
{
	InitAndLargestBlock();
	AllocAndLargestBlock();
	AllocAndFree();
	AllocAndFreeAdvanced();
	
	return 0;
}

void InitAndLargestBlock()
{
	int *pool = NULL;
	vsa_t *vsa = NULL;
	size_t pool_size = 197;
	size_t actual_size = 192;
	size_t initial_size = actual_size - sizeof(blk_header_t) - sizeof(size_t);
	puts("Flow 1: Init, Largest Block");
	puts("---------------------------");

	pool = (void*)malloc(pool_size);
	vsa = VSAInit(pool, pool_size);

	test(initial_size, VSALargestBlock(vsa), "|| Largest block after alignment");
	test(initial_size, ((blk_header_t*)vsa)->b_size, "|| Initial b_size");
    puts("---------------------------");
	free(pool);
}

void AllocAndLargestBlock()
{
	int *pool = NULL;
	vsa_t *vsa = NULL;
	vsa_t *alloc_return_address = NULL;
	size_t pool_size = 197;
	size_t actual_size = 192;
	size_t initial_size = actual_size - sizeof(blk_header_t) - sizeof(size_t);

	puts("Flow 2: Alloc, Largest Block");
	puts("----------------------------");

	pool = (void*)malloc(pool_size);
	vsa = VSAInit(pool, pool_size);

	VSAAlloc(vsa, 20);
	test(initial_size - 24 - sizeof(blk_header_t), VSALargestBlock(vsa),
		 "|| Largest block after allocation");
	test(-24, ((blk_header_t*)vsa)->b_size, "|| Initial b_size");
	VSAAlloc(vsa, 40);
	test(initial_size - 64 - (2 * sizeof(blk_header_t)), VSALargestBlock(vsa),
		 "|| Largest block after allocation");
	alloc_return_address = VSAAlloc(vsa, 400);
	(void)alloc_return_address;
    puts("----------------------------");
	free(pool);
}

void AllocAndFree()
{
	int *pool = NULL;
	vsa_t *vsa = NULL;
	vsa_t *alloc_return_address_1 = NULL;
	vsa_t *alloc_return_address_2 = NULL;
	vsa_t *alloc_return_address_3 = NULL;
	size_t pool_size = 197;
	size_t actual_size = 192;
	size_t initial_size = actual_size - sizeof(blk_header_t) - sizeof(size_t);
	size_t test_data = 0;

	puts("Flow 3: Alloc, Free");
	puts("----------------------------");

	pool = (void*)malloc(pool_size);
	vsa = VSAInit(pool, pool_size);

	alloc_return_address_1 = VSAAlloc(vsa, 20);
	test_data = initial_size - 24 - sizeof(blk_header_t);
    test(test_data, VSALargestBlock(vsa),
		 "|| Largest block after allocating 24");
	alloc_return_address_2 = VSAAlloc(vsa, 8);
	test_data -= (8 + sizeof(blk_header_t));
	test(test_data, VSALargestBlock(vsa),
		 "|| Largest block after allocating 8");
	alloc_return_address_3 = VSAAlloc(vsa, 8);
	test_data -= (8 + sizeof(blk_header_t));
	test(test_data, VSALargestBlock(vsa),
		 "|| Largest block after allocatg 8");

    VSAFree(alloc_return_address_2);
    test(test_data, VSALargestBlock(vsa),
		 "|| Largest block after freeing 2nd allocation");
    VSAFree(alloc_return_address_1);
    test(test_data, VSALargestBlock(vsa),
		 "|| Largest block after freeing 1st allocation");
    VSAFree(alloc_return_address_3);
    test(initial_size, VSALargestBlock(vsa),
		 "|| Largest block after freeing 3rd allocation");
    puts("----------------------------");
	free(pool);
}

void AllocAndFreeAdvanced()
{
	int *pool = NULL;
	vsa_t *vsa = NULL;
	vsa_t *alloc_return_address_1 = NULL;
	vsa_t *alloc_return_address_2 = NULL;
	vsa_t *alloc_return_address_3 = NULL;
	vsa_t *alloc_return_address_4 = NULL;
	vsa_t *alloc_return_address_5 = NULL;
	vsa_t *alloc_return_address_6 = NULL;
	size_t pool_size = 197;
	size_t actual_size = 192;
	size_t initial_size = actual_size - sizeof(blk_header_t) - sizeof(size_t);
	int test_data = 0;

	puts("Flow 4: Alloc, Free (Advanced)");
	puts("------------------------------");

	pool = (void*)calloc(pool_size, 1);
	vsa = VSAInit(pool, pool_size);

	alloc_return_address_1 = VSAAlloc(vsa, 2);
	alloc_return_address_2 = VSAAlloc(vsa, 4);
	alloc_return_address_3 = VSAAlloc(vsa, 6);
	alloc_return_address_4 = VSAAlloc(vsa, 8);

	VSAFree(alloc_return_address_3);

	test(72, VSALargestBlock(vsa), "|| Largest block after allocations");
	alloc_return_address_5 = VSAAlloc(vsa, 8);

	test(72, VSALargestBlock(vsa), "|| Largest block after allocations");
	/*alloc_return_address_6 = VSAAlloc(vsa, 72);
	    test(64, VSALargestBlock(vsa), "|| Largest block after allocations");


    test(0, VSALargestBlock(vsa), "|| Largest block after allocations");

	 VSAFree(alloc_return_address_2);
    test(test_data, VSALargestBlock(vsa),
		 "|| Largest block after freeing 2nd allocation");
    VSAFree(alloc_return_address_1);
    test(test_data, VSALargestBlock(vsa),
		 "|| Largest block after freeing 1st allocation");
    VSAFree(alloc_return_address_3);
    test(initial_size, VSALargestBlock(vsa),
		 "|| Largest block after freeing 3rd allocation");
  */  puts("----------------------------");
	free(pool);
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

void PrintEverything (vsa_t *vsa)
{
	ssize_t *runner = vsa;
	size_t i = 0;

	for (; i < 24; ++i, ++runner)
	{ 
		printf("%ld  \n", *runner);
	}
	printf("-----------------------\n");
}
