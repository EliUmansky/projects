/******************************************************************************/
/*               Author: Eli Umansky || Reviewer: Andrew Galuza               */
/******************************************************************************/
/*                     Exercise: DHCP || Date: 28.01.20                       */
/******************************************************************************/

#include <stdlib.h> /* malloc */
#include <stdio.h> /* sprintf */
#include <assert.h> /* assert */
#include <math.h> /* pow */
#include <string.h> /* strcpy */

#include "dhcp.h" /* ip_t */

#define TOTAL_BITS (32)
#define OCCUPIED_BITS ((32) - (dhcp->max_depth))
#define BYTE_SIZE (8)
#define USED (1)
#define NOT_USED (0)
#define ROOT (dhcp->root)
#define MAX_DEPTH (dhcp->max_depth)
#define MIN_HOST (((0x00000000) >> (MAX_DEPTH)) << (MAX_DEPTH))
#define MAX_HOST (((0xffffffff) >> (MAX_DEPTH)) << (MAX_DEPTH))

enum num_of_children { LEFT, RIGHT, NUM_OF_CHILDREN };

typedef struct trie_node trie_node_t;

struct trie_node 
{
	int is_used;
	trie_node_t *child[NUM_OF_CHILDREN];
};

struct dhcp
{
	trie_node_t *root;
	size_t max_depth;
	ip_t network_ip;
};
/* ---------------------------------------------------------------------------*/
static dhcp_t *InitNodes(dhcp_t *dhcp, size_t max_depth);
static void RecDestroyNodes(trie_node_t *root);
static trie_node_t *CreateNode();
static void CountFreeSize(trie_node_t *node, size_t *counter, size_t level);
static ip_t Alloc (trie_node_t *node, ip_t *ip, ip_t mask);
static trie_node_t *Free(trie_node_t *node, ip_t ip, ip_t mask, size_t level);
static int IsIpUsed(trie_node_t *node, ip_t ip_to_find, ip_t mask, size_t level);
static int IsNodeUsed(trie_node_t *node);
static int IsLeaf(trie_node_t *node);
/* ---------------------------------------------------------------------------*/
dhcp_t *DHCPCreate(size_t net_bits, ip_t network_ip)
{
	dhcp_t *dhcp = NULL;

	assert (TOTAL_BITS > net_bits);

	dhcp = (dhcp_t*)malloc(sizeof(dhcp_t));
	if (NULL == dhcp)
	{
		return NULL;
	}

	dhcp->max_depth = TOTAL_BITS - net_bits;
	dhcp->network_ip = (network_ip >> MAX_DEPTH) << MAX_DEPTH;
	dhcp->root = CreateNode();
	if (NULL == dhcp->root)
	{
		DHCPDestroy(dhcp);
		return NULL;
	}

	return InitNodes(dhcp, MAX_DEPTH);
}
/* ---------------------------------------------------------------------------*/
void DHCPDestroy(dhcp_t *dhcp)
{
	RecDestroyNodes(ROOT);
	memset(dhcp, 0, sizeof(dhcp_t));	
	free(dhcp);
}
/* ---------------------------------------------------------------------------*/
char *DHCPIpToStr(char *dest, ip_t ip)
{
	assert (NULL != dest);

	sprintf(dest, "%d.%d.%d.%d", (ip >> 24) % 256, (ip >> 16) % 256, 
							   	 (ip >> 8) % 256, ip % 256);

	return dest;
}
/* ---------------------------------------------------------------------------*/
ip_t DHCPStrToIp(const char *src)
{
    char *str = (char*)src;
    ip_t ret = 0;
	size_t i = 0;

	assert (NULL != src);

	while (i < 4)
	{
		ret = (ret << BYTE_SIZE) + strtol(str, &str, 10);
		++str;
		++i;
	}		

	return ret;
}
/* ---------------------------------------------------------------------------*/
size_t DHCPFreeSize(const dhcp_t *dhcp)
{
	size_t counter = 0;

	assert (NULL != dhcp);

	CountFreeSize(ROOT, &counter, MAX_DEPTH);

	return pow(2, MAX_DEPTH) - counter;
}
/* ---------------------------------------------------------------------------*/
ip_t DHCPAlloc(dhcp_t *dhcp, ip_t ip_to_alloc)
{
	ip_t ip = (ip_to_alloc << OCCUPIED_BITS) >> OCCUPIED_BITS;
	ip_t mask = pow(2, dhcp->max_depth - 1);

	assert (NULL != dhcp);

	if (USED == ROOT->is_used)
	{
		return 0;
	}

	ip = dhcp->network_ip | Alloc(ROOT, &ip, mask);

	return ip;
}
/* ---------------------------------------------------------------------------*/
void DHCPFree(dhcp_t *dhcp, ip_t ip_to_free)
{
	ip_t ip = (ip_to_free << OCCUPIED_BITS) >> OCCUPIED_BITS;
	ip_t mask = pow(2, MAX_DEPTH - 1);

	assert (NULL != dhcp);
	assert (MIN_HOST != ip_to_free);
	assert (MAX_HOST != ip_to_free);

	Free(ROOT, ip, mask, MAX_DEPTH);
}
/* ---------------------------------------------------------------------------*/
int DHCPIsIpUsed(const dhcp_t *dhcp, ip_t ip)
{
	ip_t ip_to_find = (ip << OCCUPIED_BITS) >> OCCUPIED_BITS;
	ip_t mask = 0;
	
	assert (NULL != dhcp);

	mask = pow(2, dhcp->max_depth - 1);

	return IsIpUsed(ROOT, ip_to_find, mask, MAX_DEPTH);
}
/* ------------------------------------------------------- Static functions --*/
static trie_node_t *CreateNode()
{
	trie_node_t *new_node = (trie_node_t*)malloc(sizeof(trie_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}

	new_node->is_used = NOT_USED;
	new_node->child[LEFT] = NULL;
	new_node->child[RIGHT] = NULL;

	return new_node;
}
/* ---------------------------------------------------------------------------*/
static dhcp_t *InitNodes(dhcp_t *dhcp, size_t max_depth)
{
	trie_node_t *left_child = ROOT;
	trie_node_t *right_child = ROOT;
	size_t level = 0;

	for (level = 1; level <= max_depth; ++level)
	{
		left_child->child[LEFT] = CreateNode();
		if (NULL == left_child->child[LEFT])
		{
			DHCPDestroy(dhcp);
			return NULL;
		}

		right_child->child[RIGHT] = CreateNode();
		if (NULL == right_child->child[RIGHT])
		{
			DHCPDestroy(dhcp);
			return NULL;
		}

		left_child->child[LEFT]->is_used = (max_depth == level);
		right_child->child[RIGHT]->is_used = (max_depth == level);

		left_child = left_child->child[LEFT];	
		right_child = right_child->child[RIGHT];
	}	

	return dhcp;		
}
/* ---------------------------------------------------------------------------*/
static void RecDestroyNodes(trie_node_t *node)
{
	if (NULL == node)
	{
		return;
	}

	RecDestroyNodes(node->child[LEFT]);
	RecDestroyNodes(node->child[RIGHT]);
	
	memset(node, 0, sizeof(trie_node_t));
	free(node);
}
/* ---------------------------------------------------------------------------*/
static ip_t Alloc (trie_node_t *node, ip_t *ip, ip_t mask)
{
	int side = (0 != (*ip & mask));

	if (0 == mask && NOT_USED == node->is_used)
	{
		node->is_used = USED;
		return *ip;
	}

	if (NULL != node->child[side] && USED == node->child[side]->is_used)
	{	
		*ip ^= mask;
		side = !side;
	}

	if (NULL == node->child[side])
	{
		node->child[side] = CreateNode();
		if (NULL == node->child[side])
		{
			return 0;
		}
	}

	Alloc(node->child[side], ip, mask >> 1);

	node->is_used = IsNodeUsed(node);

	return *ip;
}
/* ---------------------------------------------------------------------------*/
static int IsIpUsed(trie_node_t *node, ip_t ip_to_find, ip_t mask, size_t level)
{
	int side = (0 != (ip_to_find & mask));

	if (0 == level)
	{
		return (USED == node->is_used);
	}

	if (NULL == node->child[side])
	{
		return NOT_USED;
	}

	return IsIpUsed(node->child[side], ip_to_find, mask >> 1, level - 1); 
}
/* ---------------------------------------------------------------------------*/
static int IsNodeUsed(trie_node_t *node)
{
	if (NULL == node->child[LEFT] || NULL == node->child[RIGHT])
	{
		return 0;
	}
	
	return (USED == node->child[LEFT]->is_used && USED == node->child[RIGHT]->is_used);
}
/* ---------------------------------------------------------------------------*/
static void CountFreeSize(trie_node_t *node, size_t *counter, size_t level)
{
	if (NULL == node)
	{
		return;
	}

	if (0 == level)
	{
		++(*counter);
		return;
	}

	if (USED == node->is_used)
	{
		*counter += pow(2, level);
		return;
	}

	CountFreeSize(node->child[LEFT], counter, level - 1);
	CountFreeSize(node->child[RIGHT], counter, level - 1);	
}
/* ---------------------------------------------------------------------------*/
static trie_node_t *Free(trie_node_t *node, ip_t ip, ip_t mask, size_t level)
{
	int side = (0 != (ip & mask));

	if (0 == level || IsLeaf(node))
	{
		free(node);
		return NULL;
	}

	if (NULL == node->child[side])
	{
		return node;
	}

	node->child[side] = Free(node->child[side], ip, mask >> 1, level - 1);
	node->is_used = IsNodeUsed(node);

	return node;
}
/* ---------------------------------------------------------------------------*/
static int IsLeaf(trie_node_t *node)
{
	return (NULL == node->child[LEFT] && NULL == node->child[RIGHT]);
}

