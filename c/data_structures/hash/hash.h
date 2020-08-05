/*************************
       Hash Table
      Header File
    Author : OL80
      Date : 15/01/2020
*************************/

#ifndef HASH_H
#define HASH_H

#include <stddef.h> /* size_t */

#include "dlist.h"

typedef int (*action_func_t)(void *data, void *param);
typedef int (*match_func_t)(const void *data, const void *param);
typedef size_t(*hash_func_t)(const void *key);

typedef struct hash_table hash_table_t;

/* - Returns pointer to hash table ----------------------------------------- */
/* - table_size must be greater than 0 -------------------------------- */
/* - is_matched returns 1 if keys are matched, else 0 ---------------------- */
/* - Time complexity: O(1) ------------------------------------------------- */
hash_table_t *HashCreate(size_t table_size, match_func_t is_matched, 
						 hash_func_t hash_func);

/* - Destroys the hash table ----------------------------------------------- */
/* - hash must be a valid pointer, previously received by HashCreate ------- */
/* - Time complexity: O(n) ------------------------------------------------- */
void HashDestroy(hash_table_t *hash);

/* - Inserts data into the hash table, data must be valid to the hash table- */
/* - hash must be a valid pointer, previously received by HashCreate ------- */
/* - Time complexity: O(1) ------------------------------------------------- */
int HashInsert(hash_table_t *hash, void *data);

/* - Removes data from the hash table, data must be valid to the hash table- */
/* - hash must be a valid pointer, previously received by HashCreate ------- */
/* - Time complexity: O(1) ------------------------------------------------- */
void HashRemove(hash_table_t *hash, const void *key);

/* - Returns data matching the key if found, else returns NULL ------------- */
/* - hash must be a valid pointer, previously received by HashCreate ------- */
/* - Time complexity: O(n) ------------------------------------------------- */
void *HashFind(const hash_table_t *hash, const void *data);

/* - Returns size of the hash table ---------------------------------------- */
/* - hash must be a valid pointer, previously received by HashCreate ------- */
/* - Time complexity: O(n) ------------------------------------------------- */
size_t HashSize(const hash_table_t *hash);

/* - Returns 1 if the hash table is empty, else 0 -------------------------- */
/* - hash must be a valid pointer, previously received by HashCreate ------- */
/* - Time complexity: O(n) ------------------------------------------------- */
int HashIsEmpty(const hash_table_t *hash);

/* - Performs action_func_t on all data in the hash table ------------------ */
/* - action_func mus not be NULL ------------------------------------------- */
/* - hash must be a valid pointer, previously received by HashCreate ------- */
/* - Time complexity: O(n) ------------------------------------------------- */
int HashForEach(hash_table_t *hash, action_func_t action_func, void *param);

#endif /* HASH_H */


