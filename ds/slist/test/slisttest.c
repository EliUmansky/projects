#include <stdio.h> /* size_t */
#include <stdlib.h> /* free */

#include "slist.h"

#define RUN_TEST(test, exp) (test == exp) ? printf("OK\n") : printf("FAIL\n"); 
#define NULL_TEST(test, exp) (test != exp) ? printf("OK\n") : printf("FAIL\n"); 

static void DataPlusParam(void *data, void *param)
{	
	*(int*)data += *(int*)param;
}

static void DataMinusParam(void *data, void *param)
{	
	*(int*)data -= *(int*)param;
}

static int FindInt(const void *data, void *param)
{
	return (*(int*)data == *(int*)param);
}
static void Test();
/*static void TestMerge();
*/
int main()
{
	Test();
/*	TestMerge();
	*/

	return 0;
}

static void Test()
{
	int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7;
	int test_int = 2;
	slist_node_t *node1 = SlistCreateNode(&a, NULL);
	slist_node_t *node2 = SlistCreateNode(&b, NULL);
	slist_node_t *node3 = SlistCreateNode(&c, NULL);
	slist_node_t *node4 = SlistCreateNode(&d, NULL);
	slist_node_t *node5 = SlistCreateNode(&e, NULL);
	slist_node_t *node6 = SlistCreateNode(&f, NULL);
	slist_node_t *node7 = SlistCreateNode(&g, NULL);
	slist_node_t *test_node = NULL;	
	void *test_data = NULL;

	RUN_TEST(SlistCount(node5), 1);

	SlistInsertAfter(node1, node2);	
	SlistInsertAfter(node2, node3);
	RUN_TEST(SlistCount(node1), 3);

	SlistInsertAfter(node3, node4);
	SlistInsertAfter(node4, node5);	
	SlistInsertAfter(node5, node6);	
	SlistInsertAfter(node6, node7);	
	RUN_TEST(SlistCount(node1), 7);

	SlistForEach(node1, DataPlusParam, &test_int);
	test_data = node2 -> data;
	RUN_TEST(*(int*)test_data, 4);

	SlistForEach(node1, DataMinusParam, &test_int);
	test_data = node2 -> data;
	RUN_TEST(*(int*)test_data, 2);

	test_int = 5;	
	test_node = (slist_node_t*)SlistFind(node1, FindInt, &test_int);
	test_data = test_node -> data;
	RUN_TEST(*(int*)test_data, 5);

	test_node = (slist_node_t*)SlistFindIntersection(node2, node4);
	NULL_TEST(test_node, NULL);

	node7 = SlistFlip(node1);
	test_data = (node7 -> next) -> data;
	RUN_TEST(*(int*)test_data, 6);

	node1 = SlistFlip(node7);
	test_data = (node1 -> next) -> data;
	RUN_TEST(*(int*)test_data, 2);

	RUN_TEST(SlistHasLoop(node1), 0);

	test_node = SlistRemoveAfter(node2);
	test_data = test_node -> data;
	RUN_TEST(*(int*)test_data, 3);	
	free(test_node);
	RUN_TEST(SlistCount(node1), 6);

	SlistInsert(node4, node3);
	RUN_TEST(SlistHasLoop(node2), 0);
	SlistFreeAll(node1);
}
/*
static void TestMerge()
{
	int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7;
	int test_int = 2;
	slist_node_t *node1 = SlistCreateNode(&a, NULL);
	slist_node_t *node2 = SlistCreateNode(&b, NULL);
	slist_node_t *node3 = SlistCreateNode(&c, NULL);
	slist_node_t *node4 = SlistCreateNode(&d, NULL);
	slist_node_t *node5 = SlistCreateNode(&e, NULL);
	slist_node_t *node6 = SlistCreateNode(&f, NULL);
	slist_node_t *node7 = SlistCreateNode(&g, NULL);

	SlistInsertAfter(node1, node3);
	SlistInsertAfter(node3, node5);
	SlistInsertAfter(node5, node7);
	SlistInsertAfter(node2, node4);
	SlistInsertAfter(node4, node6);

	RUN_TEST(SlistCount(node1), 4);
	printf("%d\n", SlistCount(node1));
	RUN_TEST(SlistCount(node2), 3);
}
*/
