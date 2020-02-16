/*************************
   Singly Linked List
      Header File
    Author : OL80
      Date : 27/11/2019         
*************************/

#ifndef S_LIST_H
#define S_LIST_H

typedef struct slist_node
{
	void *data;
	struct slist_node *next;
} slist_node_t;

typedef int(*match_func_t)(const void *data, void *param);
typedef void(*action_func_t)(void *data, void *param);

#include <stddef.h> /* size_t */

/* - returnes the address of a new node containing a pointer to data ------- */
/* - data must not be null ------------------------------------------------- */
slist_node_t *SlistCreateNode(const void *data, slist_node_t *next);

/* - frees all the nodes starting from node -------------------------------- */
/* - node must not be null ------------------------------------------------- */
void SlistFreeAll(slist_node_t *head);

/* - insert node before node_next ------------------------------------------ */
/* - node must not be null ------------------------------------------------- */
/* - node_next must not be null -------------------------------------------- */
void SlistInsert(slist_node_t *where, slist_node_t *new_node);

/* - insert node after node_prev ------------------------------------------- */
/* - node must not be null ------------------------------------------------- */
/* - node_prev must not be null -------------------------------------------- */
void SlistInsertAfter(slist_node_t *where, slist_node_t *new_node);

/* - remove node ----------------------------------------------------------- */
/* - node must not be null ------------------------------------------------- */
slist_node_t *SlistRemove(slist_node_t *node);

/* - remove the next node after node --------------------------------------- */
/* - node must not be null ------------------------------------------------- */
slist_node_t *SlistRemoveAfter(slist_node_t *node);

/* - returns the number of nodes after node -------------------------------- */
/* - node must not be null ------------------------------------------------- */
size_t SlistCount(const slist_node_t *head);

/* - returns the location of the data requested within the list ------------ */
/* - func must be a custom search function appropriate to the data --------- */
/* - node must not be null ------------------------------------------------- */
slist_node_t *SlistFind(const slist_node_t *head, match_func_t find, void *param);

/* - applies func to all the list ------------------------------------------ */
/* - func must be a custom function appropriate to the data ---------------- */
/* - node must not be null ------------------------------------------------- */
void SlistForEach(slist_node_t *node, action_func_t action, void *param);

/* - flips the connection order of the list and returns the new first node - */
/* - node must not be null ------------------------------------------------- */
slist_node_t *SlistFlip(slist_node_t *head);

/* - returns 1 is the linked list contains a loop and 0 if it does not ----- */
/* - node must not be null ------------------------------------------------- */
int SlistHasLoop(const slist_node_t *node);

/* - returns the address of the intersection of two lists ------------------ */
/* - returns NULL if no intersection is found ------------------------------ */
/* - node_a and node_b must not be null ------------------------------------ */
slist_node_t *SlistFindIntersection(const slist_node_t *head_1, const slist_node_t *head_2);

#endif /* S_LIST_H */









