#include <stdio.h> /* size_t */
#include "vector.h"

#define RUN_TEST(test, exp) (test == exp) ? printf("OK\n") : printf("FAIL\n"); 
#define NULL_TEST(test, exp) (test != exp) ? printf("OK\n") : printf("FAIL\n"); 

int main()
{
	d_vector_t *dvec = NULL;
	size_t i = 0;	
	int data[10] = {1, 8, 27, 64, 125, 216, 343, 512, 729, 1000}; 
	int *value = NULL;


	dvec = DvecCreate(4, 2);
	printf("The pointer returned from DvecCreate shouldn't be NULL: ");
	NULL_TEST(dvec, NULL);
	

	printf("Size should be 0: ");
	RUN_TEST(DvecSize(dvec), 0);
	printf("Capacity should be 8: ");
	RUN_TEST(DvecCapacity(dvec), 8);

	printf("Let's start pushing some data:\n");	

	for (; i < 7; ++i)
	{
		DvecPushBack(dvec, (void*)&data[i]);
		printf("After pushing %lu elements- size should be %lu: ",
			   i + 1, i + 1);
		RUN_TEST(DvecSize(dvec), i + 1);
		printf("Capacity should be 8: ");
		RUN_TEST(DvecCapacity(dvec), 8);
	}
		
	DvecPushBack(dvec, (void*)&data[i]);
	printf("After pushing %lu elements- size should be %lu:", i + 1, i + 1);
	RUN_TEST(DvecSize(dvec), i + 1);
	printf("Capacity should be 16: ");
	RUN_TEST(DvecCapacity(dvec), 16);

	DvecReserve(dvec, 100);
	printf("After calling Reserve function with 100- capacity should be 100: ");
	RUN_TEST(DvecCapacity(dvec), 100);

	DvecPopBack(dvec);
	printf("After calling PopBack function- capacity should be 50: ");
	RUN_TEST(DvecCapacity(dvec), 50);
	printf("Size should be %lu: ", i);
	RUN_TEST(DvecSize(dvec), i);

	DvecPopBack(dvec);
	printf("After calling PopBack function again- capacity should be 25: ");
	RUN_TEST(DvecCapacity(dvec), 25);
	printf("Size should be %lu: ", i - 1);
	RUN_TEST(DvecSize(dvec), i - 1);

	DvecReserve(dvec, 4);
	printf("After calling Reserve function with 4- capacity should be 7: ");
	RUN_TEST(DvecCapacity(dvec), 7);
	printf("Size should be still %lu: ", i - 1);
	RUN_TEST(DvecSize(dvec), i - 1);

	DvecPopBack(dvec);
	DvecPopBack(dvec);
	DvecPopBack(dvec);	
	printf("After calling PopBack 3 times- capacity should be 7: ");
	RUN_TEST(DvecCapacity(dvec), 7);
	printf("Size should be 3: ");
	RUN_TEST(DvecSize(dvec), 3);

	value = (int*)(DvecGetItemAddress(dvec, 2));
	printf("The element of index 2 should be %d: ", data[2]);
	RUN_TEST(*value, data[2]);

	DvecDestroy(dvec);


	return 0;
}
