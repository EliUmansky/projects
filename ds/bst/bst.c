/***********************************************************************/
/*   Author: Eli Umansky ||| Reviewer: Daniel Oren ||| Date 06.01.20   */
/***********************************************************************/
/*       This file contains all the functions of the header bst.h      */
/***********************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "bst.h" /* bst_t */

#define SUCCESS (0)
#define DUMMY_DATA ((void*)0xBADCABAB)

typedef struct tree_node tree_node_t;
/* -------------------------------------------------------------------------- */
enum child_node_pos
{
	LEFT,
	RIGHT,
	NUMBER_OF_CHILDREN
};

struct tree_node
{
	tree_node_t *parent;
	tree_node_t *children[NUMBER_OF_CHILDREN];
	void *data;
};

struct bst
{
	tree_node_t *root_stub;
	cmp_func_t cmp_func;
	void *param;
};
/* -------------------------------------------------------------------------- */
static bst_iter_t FindNextPrev(bst_iter_t curr, enum child_node_pos position);
static void JoinNodes(bst_iter_t new_parent, bst_iter_t new_child, 
					  enum child_node_pos relative_to_parent);
/* -------------------------------------------------------------------------- */
bst_t *BSTCreate(cmp_func_t cmp_func, void *param)
{
    bst_t *bst = NULL;

    assert (NULL != cmp_func);

    bst = (bst_t*)malloc(sizeof(bst_t));
    if (NULL == bst)
    {
        return NULL;
    }

    bst->root_stub = (tree_node_t*)malloc(sizeof(tree_node_t));
    if (NULL == bst->root_stub)
    {
        free(bst);
        return NULL;
    }

    bst->cmp_func = cmp_func;
    bst->param = param;

    bst->root_stub->parent = NULL;
    bst->root_stub->children[LEFT] = NULL;
    bst->root_stub->children[RIGHT] = NULL;
    bst->root_stub->data = DUMMY_DATA;

    return bst;
}
/* -------------------------------------------------------------------------- */
void BSTDestroy(bst_t *bst)
{
    bst_iter_t destroyer = NULL, runner = NULL;

    assert (NULL != bst);

    destroyer = BSTBegin(bst);

    while (!BSTIsEmpty(bst))
    {
		runner = BSTNext(destroyer);
        BSTRemove(destroyer);
        destroyer = runner;
    }

	free(bst->root_stub);
	free(bst);
}
/* -------------------------------------------------------------------------- */
int BSTIsEmpty(const bst_t *bst)
{
    assert (NULL != bst);

    return (BSTIsSameIter(BSTBegin(bst), BSTEnd(bst)));
}
/* -------------------------------------------------------------------------- */
size_t BSTSize(const bst_t *bst)
{
	bst_iter_t runner = BSTBegin(bst); 
    int counter = 0;

    assert (NULL != bst);

	while (!BSTIsSameIter(runner, BSTEnd(bst)))
	{
		++counter;
		runner = BSTNext(runner);
	}

    return counter;
}
/* -------------------------------------------------------------------------- */
bst_iter_t BSTInsert(bst_t *bst, void *data)
{
    tree_node_t *new_node = NULL;
    bst_iter_t runner = NULL;
    enum child_node_pos next_pos = LEFT;

    assert (NULL != bst);
    assert (NULL != data);

    new_node = (tree_node_t*)malloc(sizeof(tree_node_t));
    if (NULL == new_node)
    {
        return NULL;
    }

    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->data = data;

    runner = BSTEnd(bst);

    while (NULL != runner->children[next_pos])
    {
        runner = runner->children[next_pos];
        next_pos = 
		(bst->cmp_func(data, BSTGetData(runner), bst->param)) > 0 ? RIGHT : LEFT;
    }

	JoinNodes(new_node, runner, next_pos);    

    runner = new_node;

    return runner;
}
/* -------------------------------------------------------------------------- */
void *BSTRemove(bst_iter_t bst_iter)
{
	bst_iter_t rem = bst_iter;
	void *data = BSTGetData(bst_iter);
	enum child_node_pos relative_to_parent = LEFT;

	assert (DUMMY_DATA != BSTGetData(rem));

	relative_to_parent = 
	(BSTIsSameIter(bst_iter, (bst_iter->parent)->children[LEFT])) ? LEFT : RIGHT;	

	if (NULL != rem->children[LEFT])
	{
		JoinNodes(rem->children[RIGHT], BSTPrev(rem), RIGHT);
		JoinNodes(rem->children[LEFT], rem->parent, relative_to_parent);
	}
	else
	{
		JoinNodes(rem->children[RIGHT], rem->parent, relative_to_parent);
	}

	free(rem);

	return data;
}

/* -------------------------------------------------------------------------- */
bst_iter_t BSTBegin(const bst_t *bst)
{
    bst_iter_t begin_iter = NULL;

    assert (NULL != bst);

    begin_iter = (bst->root_stub);

    while (NULL != begin_iter->children[LEFT])
    {
        begin_iter = begin_iter->children[LEFT];
    }

    return begin_iter;
}
/* -------------------------------------------------------------------------- */
bst_iter_t BSTEnd(const bst_t *bst)
{
    assert (NULL != bst);

    return bst->root_stub;
}
/* -------------------------------------------------------------------------- */
bst_iter_t BSTNext(bst_iter_t curr)
{
	enum child_node_pos position = RIGHT;
    
	assert (NULL != curr);
	assert (DUMMY_DATA != BSTGetData(curr));

	return FindNextPrev(curr, position);  
}
/* -------------------------------------------------------------------------- */
bst_iter_t BSTPrev(bst_iter_t curr)
{
	enum child_node_pos position = LEFT;

    assert (NULL != curr);

    return FindNextPrev(curr, position);
}
/* -------------------------------------------------------------------------- */
int BSTIsSameIter(bst_iter_t bst_iter_1, bst_iter_t bst_iter_2)
{
    return (bst_iter_1 == bst_iter_2);
}
/* -------------------------------------------------------------------------- */
void *BSTGetData(bst_iter_t bst_iter)
{
    assert (NULL != bst_iter);

    return bst_iter->data;
}
/* -------------------------------------------------------------------------- */
bst_iter_t BSTFind(bst_t *bst, void *data)
{
	bst_iter_t runner = NULL;
	int result = 0;
	enum child_node_pos next_pos = LEFT;

	assert (NULL != bst);
	
	runner = BSTEnd(bst);

    while (NULL != runner->children[next_pos])
    {
        runner = runner->children[next_pos];
        result = bst->cmp_func(data, BSTGetData(runner), bst->param);
		if (0 == result)
		{
			return runner;
		}

		next_pos = (0 < result) ? RIGHT : LEFT;
    }

	return BSTEnd(bst);
}
/* -------------------------------------------------------------------------- */
int BSTForEach(bst_iter_t start, bst_iter_t end,
			   action_func_t action_func, void *param)
{
	bst_iter_t runner = NULL;
	int status = SUCCESS;

	assert (NULL != action_func);

	runner = start;

	while (!BSTIsSameIter(runner, end) && SUCCESS == status)
	{
		status = action_func(BSTGetData(runner), param);
		runner = BSTNext(runner);
	}

	return status;
}
/* -------------------------------------------------------------------------- */
static void JoinNodes(bst_iter_t new_child, bst_iter_t new_parent, 
					  enum child_node_pos relative_to_parent)
{
	new_parent->children[relative_to_parent] = new_child;
	
	if (NULL != new_child)
	{	
		new_child->parent = new_parent;
	}
}
/* -------------------------------------------------------------------------- */
static bst_iter_t FindNextPrev(bst_iter_t curr, enum child_node_pos position)
{
	bst_iter_t runner = curr;

	if (NULL != runner->children[position])
    {
        runner = runner->children[position];
	
		while (NULL != runner->children[!position])
		{
			runner = runner->children[!position];
		}

		return runner;
	}
	else
	{
		while (BSTIsSameIter(runner,(runner->parent)->children[position]) &&
		       NULL != (runner->parent)->parent)
		{
			runner = runner->parent;
		}
	
		return runner->parent;
	}
}	
