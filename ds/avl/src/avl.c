/******************************************************************************/
/*                Author: Eli Umansky || Reviewer: Max Zazumin                */
/******************************************************************************/
/*                    Exercise: AVL Tree || Date: 14.01.20                    */
/******************************************************************************/
#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "avl.h" /* avl_t */

#define SUCCESS (0)
#define MALLOC_FAIL (-1)
#define ROOT_L_CHILD (root->children[LEFT])
#define ROOT_R_CHILD (root->children[RIGHT])
#define MAX_OF(x, y) (((x) > (y)) ? (x) : (y))

enum child_node_pos {LEFT, RIGHT, NUM_OF_CHILDREN};
typedef struct tree_node tree_node_t;

struct avl
{
	tree_node_t *root;
	cmp_func_t cmp_func;
	void *param;
};

struct tree_node
{
	size_t height;
	tree_node_t *children[NUM_OF_CHILDREN];
	void *data;
};

static int RecForEach(tree_node_t *root, action_func_t action_func, void *param);
static void RecDestroyNode(tree_node_t *root);
static tree_node_t *RecInsert(tree_node_t *root, tree_node_t *new_node, 
					  cmp_func_t cmp_func, void *param);
static int SizeCounter(void *data, void *param);
static void *RecFind(tree_node_t *root, cmp_func_t cmp_func, 
							void *key_data, void *param);
static void FindNodeToRemove(tree_node_t *root, cmp_func_t cmp_func, void *data, 
						     void *param, tree_node_t **parent_of_removed);
static void RemoveAndUpdateHeight(tree_node_t *root, 
								  tree_node_t **potential_parent);
static void UpdateHeight(tree_node_t *root);
static tree_node_t *CreateNode(void *data);
static void *GetDataAndRemove(tree_node_t *root, tree_node_t **parent_of_removed);
static int FindHeavySide(tree_node_t *root);
static tree_node_t *Rebalance(tree_node_t *root);
static tree_node_t *RotateLeft(tree_node_t *root, tree_node_t *pivot);
static tree_node_t *RotateRight(tree_node_t *root, tree_node_t *pivot);
static int DoesHaveRightChild(tree_node_t *root);

avl_t *AVLCreate(cmp_func_t cmp_func, void *param)
{
	avl_t *avl = NULL;

	assert (NULL != cmp_func);

	avl = (avl_t*)malloc(sizeof(avl_t));
	if (NULL == avl)
	{
		return NULL;
	}

	avl->root = NULL;
	avl->cmp_func = cmp_func;
	avl->param = param;

	return avl;
}

void AVLDestroy(avl_t *avl)
{
	assert (NULL != avl);

	if (!AVLIsEmpty(avl))
	{
		RecDestroyNode(avl->root);
	}

	free(avl);
}

static void RecDestroyNode(tree_node_t *root)
{
	if (NULL != ROOT_L_CHILD)
	{
		RecDestroyNode(ROOT_L_CHILD);
	}
	if (NULL != ROOT_R_CHILD)
	{
		RecDestroyNode(ROOT_R_CHILD);
	}	
	
	free(root);
}

int AVLInsert(avl_t *avl, void *data)
{
	tree_node_t *new_node = NULL, *new_root = NULL;		

	assert (NULL != avl);
	
	new_node = CreateNode(data);
	if (NULL == new_node)
	{
		return MALLOC_FAIL;
	}

	if (AVLIsEmpty(avl))
	{
		avl->root = new_node;
	}
	else
	{
		new_root = RecInsert(avl->root, new_node, avl->cmp_func, avl->param);
		avl->root = new_root;
	}

	return SUCCESS;
}

static tree_node_t *RecInsert(tree_node_t *root, tree_node_t *new_node, 
					  cmp_func_t cmp_func, void *param)
{
	int next_pos = (0 < cmp_func(new_node->data, root->data, param));
	tree_node_t *new_root = NULL;

	if (NULL == root->children[next_pos])	
	{
		root->children[next_pos] = new_node;
	}
	else
	{
		new_root = RecInsert(root->children[next_pos], new_node, cmp_func, param);
		root->children[next_pos] = new_root;
	}	

	UpdateHeight(root);

	return Rebalance(root);
}

void AVLRemove(avl_t *avl, void *data)
{	
	assert (NULL != avl);

	if (AVLIsEmpty(avl))
	{
		return;
	}
	else if (1 == AVLSize(avl) && 0 == avl->cmp_func(data, avl->root->data, avl->param))
	{
		free(avl->root);
		avl->root = NULL;
		return;
	}
	else
	{	
		FindNodeToRemove(avl->root, avl->cmp_func, data, avl->param, &avl->root);
		UpdateHeight(avl->root);
		avl->root = Rebalance(avl->root);
	}
}

static void FindNodeToRemove(tree_node_t *root, cmp_func_t cmp_func, void *data, 
						     void *param, tree_node_t **parent_of_removed)
{	
	int next_step = 0, cmp_val = cmp_func(data, root->data, param);

	if (0 == cmp_val)
	{
		RemoveAndUpdateHeight(root, parent_of_removed);
	}	
	else
	{
		next_step = (0 < cmp_val) ? RIGHT : LEFT;
	
		if (NULL == root->children[next_step])
		{
			return;
		}
		else	
		{
			parent_of_removed = &root;			
			FindNodeToRemove(root->children[next_step], cmp_func, data, param,
							 parent_of_removed);				
		}
	}

	UpdateHeight(*parent_of_removed);
	(*parent_of_removed)->children[next_step] = 
	Rebalance((*parent_of_removed)->children[next_step]);
}

static void RemoveAndUpdateHeight(tree_node_t *root, 
								  tree_node_t **parent_of_removed)
{
	int side = 0;
	tree_node_t *node_to_remove = NULL;

	if (DoesHaveRightChild(root))
	{			
		if (NULL == ROOT_R_CHILD->children[LEFT])
		{
			node_to_remove = ROOT_R_CHILD;
			root->data = ROOT_R_CHILD->data;				
			ROOT_R_CHILD = ROOT_R_CHILD->children[RIGHT];
			free(node_to_remove);
			UpdateHeight(root);
		}
		else
		{
			root->data = GetDataAndRemove(ROOT_R_CHILD, &root);
		}
	}
	else
	{
		side = (root == (*parent_of_removed)->children[RIGHT]);
		(*parent_of_removed)->children[side] = ROOT_L_CHILD;			
		free(root);
		UpdateHeight(*parent_of_removed);
	}
}

void *AVLFind(const avl_t *avl, void *key_data)
{
	assert (NULL != avl);

	return RecFind(avl->root, avl->cmp_func, key_data, avl->param);
}

static void *RecFind(tree_node_t *root, cmp_func_t cmp_func, 
							void *key_data, void *param)
{
	int cmp_val = 0, next_step = 0;	
	
	if (NULL == root)
	{
		return NULL;
	}

	cmp_val = cmp_func(key_data, root->data, param);

	if (0 == cmp_val)
	{
		return root->data;
	}
	else
	{
		next_step = (0 < cmp_val);
	}

	return RecFind(root->children[next_step], cmp_func, key_data, param);
}

size_t AVLSize(const avl_t *avl)
{
	size_t counter = 0;

	assert (NULL != avl);

	AVLForEach((avl_t*)avl, SizeCounter, &counter);

	return counter; 
}
	
int AVLIsEmpty(const avl_t *avl)
{
	assert (NULL != avl);

	return (NULL == avl->root);
}

int AVLForEach(avl_t *avl, action_func_t action_func, void *param)
{
	assert (NULL != avl);

	if (AVLIsEmpty(avl))
	{
		return 0;
	}

	return RecForEach(avl->root, action_func, param); 
}

static int RecForEach(tree_node_t *root, action_func_t action_func, void *param)
{
	int status = SUCCESS;	

	if (NULL != ROOT_L_CHILD && SUCCESS == status)
	{
		status = RecForEach(ROOT_L_CHILD, action_func, param);
	}
	if (SUCCESS == status)
	{		
		status = action_func(root->data, param);
	}
	if (NULL != ROOT_R_CHILD && SUCCESS == status)
	{
		status = RecForEach(ROOT_R_CHILD, action_func, param);
	}	

	return status;
}

size_t AVLHeight(const avl_t *avl)
{
	assert (NULL != avl);

	return (AVLIsEmpty(avl)) ? 0 : (avl->root)->height;
}

static tree_node_t *CreateNode(void *data)
{
	tree_node_t *new_node = (tree_node_t*)malloc(sizeof(tree_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}

	new_node->height = 1;
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	new_node->data = data;

	return new_node;
}

static void UpdateHeight(tree_node_t *root)
{
	size_t left_height = 0, right_height = 0;

	if (NULL == root)
	{
		return;
	}	

	left_height = (NULL == ROOT_L_CHILD) ? 0 : ROOT_L_CHILD->height, 
	right_height = (NULL == ROOT_R_CHILD) ? 0 : ROOT_R_CHILD->height;	

	root->height =  1 + MAX_OF(left_height, right_height);
}

static void *GetDataAndRemove(tree_node_t *root, tree_node_t **parent_of_removed)
{
	void *data_to_replace = NULL;	
	
	if (NULL == ROOT_L_CHILD)
	{
		data_to_replace = root->data;
		(*parent_of_removed)->children[LEFT] = ROOT_R_CHILD;	
		free(root);

		return data_to_replace;
	}
	
	parent_of_removed = &root;
	data_to_replace = GetDataAndRemove(ROOT_L_CHILD, parent_of_removed);
	UpdateHeight(root);
	ROOT_L_CHILD = Rebalance(ROOT_L_CHILD);

	return data_to_replace;
}
	
static tree_node_t *Rebalance(tree_node_t *root)
{
	int balance = FindHeavySide(root);

	if (NULL == root)
	{
		return NULL;
	}	

	if (1 < balance)
	{
		balance = FindHeavySide(ROOT_R_CHILD);	
		if (0 <= balance)
		{
			return RotateLeft(root, ROOT_R_CHILD);
		}
		else
		{
			ROOT_R_CHILD = RotateRight(ROOT_R_CHILD, ROOT_R_CHILD->children[LEFT]);
			return RotateLeft(root, ROOT_R_CHILD);
		}
	}
	else if (-1 > balance)
	{
		balance = FindHeavySide(ROOT_L_CHILD);	
		if (0 >= balance)
		{
			return RotateRight(root, ROOT_L_CHILD);		
		}
		else
		{
			ROOT_L_CHILD = RotateLeft(ROOT_L_CHILD, ROOT_L_CHILD->children[RIGHT]);
			return RotateRight(root, ROOT_L_CHILD);
		}	
	}
	else
	{
		return root;
	}
}

static tree_node_t *RotateRight(tree_node_t *root, tree_node_t *pivot)
{
	root->children[LEFT] = pivot->children[RIGHT];
	pivot->children[RIGHT] = root;

	UpdateHeight(root);
	UpdateHeight(pivot);

	return pivot;
}

static tree_node_t *RotateLeft(tree_node_t *root, tree_node_t *pivot)
{
	root->children[RIGHT] = pivot->children[LEFT];
	pivot->children[LEFT] = root;

	UpdateHeight(root);
	UpdateHeight(pivot);

	return pivot;
}

static int FindHeavySide(tree_node_t *root)
{	
	if (NULL == root || (NULL == ROOT_L_CHILD && NULL == ROOT_R_CHILD))
	{
		return 0;
	}
	else if (NULL == ROOT_L_CHILD)
	{	
		return (int)ROOT_R_CHILD->height;
	}
	else if (NULL == ROOT_R_CHILD)
	{	
		return -(int)(ROOT_L_CHILD->height);
	}
	else
	{
		return ((int)ROOT_R_CHILD->height - (int)ROOT_L_CHILD->height);
	}
}

static int DoesHaveRightChild(tree_node_t *root)
{
	return (NULL != ROOT_R_CHILD);
}


static int SizeCounter(void *data, void *param)
{
	(void)data;	
	++(*(int*)param);

	return 0;
}

