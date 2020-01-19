/*************************
        AVL Tree
      Header File
    Author : OL80
      Date : 12/01/2020
*************************/

#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stddef.h> /* size_t */

typedef struct avl avl_t;
typedef int (*cmp_func_t)(const void *data_new, const void *data, void *param);
typedef int (*action_func_t)(void *data, void *param);

/* - returns a pointer to a new avl ---------------------------------------- */
/* - cmp_func must be a valid function that can compare avl intended data -- */
/* - param is a param to be passed to cmp_func ----------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
avl_t *AVLCreate(cmp_func_t cmp_func, void *param);

/* - destroys an existing avl ---------------------------------------------- */
/* - avl must not be NULL -------------------------------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
void AVLDestroy(avl_t *avl);

/* - inserts data as a new element into avl and rebalances the tree -------- */
/* - avl must not be NULL -------------------------------------------------- */
/* - complexity O(log n) --------------------------------------------------- */
int AVLInsert(avl_t *avl, void *data);

/* - removes data from the tree and rebalances it -------------------------- */
/* - avl must not be NULL -------------------------------------------------- */
/* - complexity O(log n) --------------------------------------------------- */
void AVLRemove(avl_t *avl, void *data);

/* - returns the number of elements currently in avl ----------------------- */
/* - avl must not be NULL -------------------------------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
size_t AVLSize(const avl_t *avl);

/* - returns 1 if avl is empty and 0 if not -------------------------------- */
/* - avl must not be NULL -------------------------------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
int AVLIsEmpty(const avl_t *avl);

/* - returns the data matching a given key --------------------------------- */
/* - avl must not be NULL -------------------------------------------------- */
/* - complexity O(log n) --------------------------------------------------- */
void *AVLFind(const avl_t *avl, void *key_data);

/* - performs an action on all the elements in avl ------------------------- */
/* - returns 0 on success. forwards the return of the action_func on fail -- */
/* - action_func must be a valid action func ------------------------------- */
/* - action_func must NOT change the sorting key! -------------------------- */
/* - complexity O(n) ------------------------------------------------------- */
int AVLForEach(avl_t *avl, action_func_t action_func, void *param);

/* - returns the height of the avl ----------------------------------------- */
/* - avl must not be NULL -------------------------------------------------- */
/* - complexity O(1) ------------------------------------------------------- */
size_t AVLHeight(const avl_t *avl);

#endif /* AVL_TREE_H */

/*

enum child_node_pos { LEFT,	RIGHT, NUM_OF_CHILDREN };

typedef struct tree_node tree_node_t

struct tree_node
{
	size_t height;
	tree_node_t *children[NUM_OF_CHILDREN];
	void *data;
};

struct avl
{
	tree_node_t *root;
	cmp_func_t cmp_func;
	void *param;
};
*/
