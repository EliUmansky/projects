#include <stdio.h> /* size_t */
#include "vector.h"

#define RUN_TEST(test, exp) (test == exp) ? printf("OK\n") : printf("FAIL\n"); 
#define NULL_TEST(test, exp) (test != exp) ? printf("OK\n") : printf("FAIL\n"); 

int main()
{
	d_vector_t *dvec = NULL;
	size_t i = 0;	
	int data1[10] = {1, 8, 27, 64, 125, 216, 343, 512, 729, 1000}; 
	float data2[10] = {1.1, 8.2, 27.3, 64.4, 125.5, 216.6, 343.7, 512.8, 
				       729.9, 1000.0};
	int *value = NULL;

	dvec = DvecCreate(4, 2);
	printf("The pointer returned from DvecCreate shouldn't be NULL: ");
	NULL_TEST(DvecCreate(4, 2), NULL);
	

	printf("Size should be 0: ");
	RUN_TEST(DvecSize(dvec), 0);
	printf("Capacity should be 8: ");
	RUN_TEST(DvecCapacity(dvec), 8);

	printf("Let's start pushing some data:\n");	

	for (; i < 6; ++i)
	{
		DvecPushBack(dvec, (void*)&data1[i]);
		printf("After pushing %d elements- size should be %ld: ", i + 1);
		RUN_TEST(DvecSize(dvec), i + 1);
		printf("Capacity should be 8: ");
		RUN_TEST(DvecCapacity(dvec), 8);
	}
/*
	DvecPushBack(dvec, (void*)&num2);
	printf("Size is %ld\n", DvecSize(dvec));
	printf("Capacity is %ld\n", DvecCapacity(dvec));

	DvecPushBack(dvec, (void*)&num1);
	printf("Size is %ld\n", DvecSize(dvec));
	printf("Capacity is %ld\n", DvecCapacity(dvec));

	DvecPushBack(dvec, (void*)&num2);
	printf("Size is %ld\n", DvecSize(dvec));
	printf("Capacity is %ld\n", DvecCapacity(dvec));

	value = DvecGetItemAddress(dvec, 1);
	printf("%d", *value);
	value = DvecGetItemAddress(dvec, 2);
	printf("%d", *value);

	DvecDestroy(dvec);
	printf("Size is %ld\n", DvecSize(dvec));
	printf("Capacity is %ld\n", DvecCapacity(dvec));*/

	return 0;
}
