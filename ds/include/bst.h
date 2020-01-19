/*************************
   Binary Search Tree
      Header File
    Author : OL80
      Date : 02/01/2020
*************************/

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <stddef.h> /* size_t */

typedef struct tree_node *bst_iter_t;
typedef struct bst bst_t;
typedef int (*cmp_func_t)(const void *data_new, const void *data, void *param);
typedef int (*action_func_t)(void *data, void *param);

/* - returns a pointer to a new bst ---------------------------------------- */
/* - cmp_func must be a valid function that can compare bst indended data -- */
/* - param is a param to be passed to cmp_func ----------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
bst_t *BSTCreate(cmp_func_t cmp_func, void *param);

/* - destroys an existing bst ---------------------------------------------- */
/* - bst must not be NULL -------------------------------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
void BSTDestroy(bst_t *bst);

/* - returns 1 if bst is empty and 0 if not -------------------------------- */
/* - bst must not be NULL -------------------------------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
int BSTIsEmpty(const bst_t *bst);

/* - returns the number of elements currently in bst ----------------------- */
/* - bst must not be NULL -------------------------------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
size_t BSTSize(const bst_t *bst);

/* - inserts data as a new element into bst and returns an iterator to it -- */
/* - bst must not be NULL -------------------------------------------------- */
/* - complexity O(log n) worst case O(n) ----------------------------------- */
bst_iter_t BSTInsert(bst_t *bst, void *data);

/* - removes bst_iter from the bst and returns its data -------------------- */
/* - bst_iter must be a valid iterator of a bst ---------------------------- */
/* - complexity O(log n) worst case O(n) ----------------------------------- */
void *BSTRemove(bst_iter_t bst_iter);

/* - returns the first element in the bst ---------------------------------- */
/* - bst must not be NULL -------------------------------------------------- */
/* - complexity O(log n) worst case O(n) ----------------------------------- */
bst_iter_t BSTBegin(const bst_t *bst);

/* - returns the END of the bst -------------------------------------------- */
/* - bst must not be NULL -------------------------------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
bst_iter_t BSTEnd(const bst_t *bst);

/* - returns the next element after curr ----------------------------------- */
/* - curr must be a valid iterator of a bst -------------------------------- */
/* - complexity O(log n) worst case O(n) ----------------------------------- */
bst_iter_t BSTNext(bst_iter_t curr);

/* - returns the previous element before curr ------------------------------ */
/* - curr must be a valid iterator of a bst -------------------------------- */
/* - complexity O(log n) worst case O(n) ----------------------------------- */
bst_iter_t BSTPrev(bst_iter_t curr);

/* - returns 1 if bst_iter_1 and bst_iter_2 are the same iterator ---------- */
/* - bst_iter_1 and bst_iter_2 must be valid iterators of a bst ------------ */
/* - complexity O(1) ------------------------------------------------------- */
int BSTIsSameIter(bst_iter_t bst_iter_1, bst_iter_t bst_iter_2);

/* - returns the data from the element bst_iter is set to ------------------ */
/* - bst_iter must be a valid iterator of a bst ---------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
void *BSTGetData(bst_iter_t bst_iter);

/* - returns an iterator or the first element found ------------------------ */
/* - bst must not be NULL -------------------------------------------------- */
/* - complexity O(log n) worst case O(n) ----------------------------------- */
bst_iter_t BSTFind(bst_t *bst, void *data);

/* - performs an action on all the elements in bst from start to end ------- */
/* - returns 0 on success. forwards the return of the action_func on fail -- */
/* - action_func must be a valid action func ------------------------------- */
/* - action_func must NOT change the sorting key! -------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
int BSTForEach(bst_iter_t start, bst_iter_t end,
			   action_func_t action_func, void *param);

#endif /* BINARY_SEARCH_TREE_H */

/*
#define DUMMY_DATA (0xBADCABAB);

enum child_node_pos { LEFT,	RIGHT, NUM_OF_CHILDREN };

typedef struct tree_node tree_node_t;

struct tree_node
{
	tree_node_t *parent;
	tree_node_t *children[NUM_OF_CHILDREN];
	void *data;
};

struct bst
{
	tree_node_t *root_stub;
	cmp_func_t cmp_func;
	void *param;
};
*/
