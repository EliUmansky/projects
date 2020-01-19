/******************************************************************************/
/*                Author: Eli Umansky || Reviewer: Ben Shomroni               */
/******************************************************************************/
/*                   Exercise: Hash Table || Date: 18.01.20                   */
/******************************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "hash.h" /* hash_table_t */

#define SUCCESS (0)
#define MALLOC_FAIL (-1)

struct hash_table
{
	dlist_t **hash_arr;
	match_func_t is_matched;
	hash_func_t hash_func;
	size_t table_size;
};

static int InitHashArr(dlist_t **hash_arr, size_t table_size);
static dlist_iter_t FindNode(hash_table_t *hash, const void *data, size_t index);
static int FindIndex(hash_table_t *hash, const void *data);

hash_table_t *HashCreate(size_t table_size, match_func_t is_matched,
						 hash_func_t hash_func)
{
	hash_table_t *hash = NULL;
	int status = SUCCESS;

	assert (0 < table_size);
	assert (NULL != is_matched);
	assert (NULL != hash_func);

	hash = (hash_table_t*)malloc(sizeof(hash_table_t));
	if (NULL == hash)
	{
		return NULL;
	}

	hash->table_size = table_size;
	hash->is_matched = is_matched;
	hash->hash_func = hash_func;
	hash->hash_arr = (dlist_t**)malloc(table_size * sizeof(dlist_t*));
	if (NULL == hash->hash_arr)
	{
		free(hash);
		return NULL;
	}

	status = InitHashArr(hash->hash_arr, table_size);
	if (SUCCESS == status)
	{
		return hash;
	}
	else
	{
		free(hash->hash_arr);
		free(hash);
		return NULL;
	}
}

void HashDestroy(hash_table_t *hash)
{
	size_t i = 0;

	assert (NULL != hash);

	for (i = 0; i < hash->table_size; ++i)
	{
		DListDestroy(hash->hash_arr[i]);
	}

	free(hash->hash_arr);
	free(hash);
}

size_t HashSize(const hash_table_t *hash)
{
	size_t i = 0, size_counter = 0;

	assert (NULL != hash);

	for (i = 0; i < hash->table_size; ++i)
	{
		size_counter += DListSize(hash->hash_arr[i]);
	}

	return size_counter;
}

int HashIsEmpty(const hash_table_t *hash)
{
	assert (NULL != hash);

	return (0 == HashSize(hash));
}

int HashForEach(hash_table_t *hash, action_func_t action_func, void *param)
{
	int status = SUCCESS;
	size_t i = 0;

	assert (NULL != hash);

	for (i = 0; i < hash->table_size && SUCCESS == status; ++i)
	{
		status = DListForEach(DListBegin(hash->hash_arr[i]),
				 DListEnd(hash->hash_arr[i]), action_func, param);
	}

	return status;
}

int HashInsert(hash_table_t *hash, void *data)
{
	dlist_iter_t new_node = NULL;
	size_t index = 0;

	assert (NULL != hash);

	index = FindIndex(hash, data);

	if (NULL != HashFind(hash, data))
	{
		HashRemove(hash, data);
	}
	new_node = DListPushFront(hash->hash_arr[index], data);

	return (DListIsSameIterator(new_node, DListEnd(hash->hash_arr[index])))
			? MALLOC_FAIL : SUCCESS;
}

void *HashFind(const hash_table_t *hash, const void *data)
{
	size_t index = 0;
	dlist_iter_t data_holder = NULL;

	assert (NULL != hash);

	index = FindIndex((hash_table_t*)hash, data);
	data_holder = FindNode((hash_table_t*)hash, data, index);

	return (DListIsSameIterator(data_holder, DListEnd(hash->hash_arr[index]))) ?
			NULL : DListGetData(data_holder);
}

void HashRemove(hash_table_t *hash, const void *key)
{
	size_t index = 0;
	dlist_iter_t node_to_remove = NULL;

	assert (NULL != hash);

	index = FindIndex(hash, key);
	node_to_remove = FindNode(hash, key, index);

	DListRemove(node_to_remove);
}

static int InitHashArr(dlist_t **hash_arr, size_t table_size)
{
	size_t i = 0;

	for (i = 0; i < table_size; ++i)
	{
		hash_arr[i] = DListCreate();
		if (NULL == hash_arr[i])
		{
			for (; 0 != i; --i)
			{
				DListDestroy(hash_arr[i]);
			}
			return MALLOC_FAIL;
		}
	}

	return SUCCESS;
}

static dlist_iter_t FindNode(hash_table_t *hash, const void *data, size_t index)
{
	return DListFind(DListBegin(hash->hash_arr[index]),
							DListEnd(hash->hash_arr[index]),
							hash->is_matched, data);
}

static int FindIndex(hash_table_t *hash, const void *data)
{
	return hash->hash_func(data) % hash->table_size;
}
