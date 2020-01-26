/***********************************************************************/
/*  Author: Eli Umansky ||| Reviewer: Andrew Galuza ||| Date 04.12.19  */
/***********************************************************************/
/*      This file contains all the functions of the header cbuff.h     */
/***********************************************************************/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <sys/types.h> /* ssize_t */
#include <string.h> /* memcpy */

#include "cbuff.h"	/* cbuff_t */

#define START (buffer->start)
#define CAPACITY (buffer->capacity)
#define READ_INDEX (buffer->read_index)
#define SIZE (buffer->size)
#define BYTES_FROM_END ((size_t)(START + CAPACITY) - (size_t)begin_copy_address)

typedef char byte; 

struct cbuff
{
	byte *start;
	size_t capacity;
	size_t read_index;
	size_t size;
};

static void CopyDataToBuffer(cbuff_t *buffer, byte *begin_copy_address,
 							 const void *data, size_t bytes);
static size_t CopyDataFromBuffer(cbuff_t *buffer, void *data,
 								 byte *begin_copy_address, size_t bytes);

cbuff_t *CbuffCreate(size_t capacity)
{
	cbuff_t *buffer = NULL;

	assert(0 < capacity);

	buffer = (cbuff_t*)malloc(capacity + sizeof(cbuff_t));
	if (NULL == buffer)
	{
		return NULL;
	}

	START = (byte*)buffer + sizeof(cbuff_t);
	CAPACITY = capacity;
	READ_INDEX = 0;
	SIZE = 0;

	return buffer;
}

void CbuffDestroy(cbuff_t *buffer)
{
	assert(NULL != buffer);

	free(buffer);
}

ssize_t CbuffWrite(cbuff_t *buffer, const void *data, size_t bytes)
{
	byte *begin_copy_address = NULL;
	size_t bytes_to_write = 0;

	assert(NULL != buffer);
	assert(NULL != data);

	begin_copy_address = START + ((READ_INDEX + SIZE) % CAPACITY);
	bytes_to_write = (bytes > CbuffFreeSpace(buffer)) ? CbuffFreeSpace(buffer)
					  : bytes;
	
	CopyDataToBuffer(buffer, begin_copy_address, data, bytes_to_write);

	SIZE += bytes_to_write;

	return bytes_to_write;
}

ssize_t CbuffRead(cbuff_t *buffer, void *data, size_t bytes)
{
	byte *begin_copy_address = NULL;	
	size_t bytes_to_read = 0;
	
	assert(NULL != buffer);
	assert(NULL != data);
	
	begin_copy_address = START + READ_INDEX;
	bytes_to_read = (bytes > SIZE) ? SIZE : bytes;

	READ_INDEX = CopyDataFromBuffer(buffer, data, begin_copy_address, bytes);
	SIZE -= bytes_to_read;

	return bytes_to_read;		
}

int CbuffIsEmpty(const cbuff_t *buffer)
{
	assert(NULL != buffer);

	return (0 == buffer->size);	
}

size_t CbuffFreeSpace(const cbuff_t *buffer)
{
	assert(NULL != buffer);

	return (buffer->capacity - buffer->size);
}

size_t CbuffCapacity(const cbuff_t *buffer)
{
	assert(NULL != buffer);

	return buffer->capacity;	
}

static void CopyDataToBuffer(cbuff_t *buffer, byte *begin_copy_address, const void *data,
					 size_t bytes)
{
	if (BYTES_FROM_END >= bytes)
	{
		memcpy(begin_copy_address, data, bytes);
	}

	else
	{
		memcpy(begin_copy_address, data, BYTES_FROM_END);
		memcpy(START, (byte*)data + BYTES_FROM_END, bytes - BYTES_FROM_END);
	}
}

static size_t CopyDataFromBuffer(cbuff_t *buffer, void *data, byte *begin_copy_address,
					 size_t bytes)
{
	if (BYTES_FROM_END >= bytes)
	{
		memcpy(data, begin_copy_address, bytes);
	}

	else
	{
		memcpy(data, begin_copy_address, BYTES_FROM_END);
		memcpy((byte*)data + BYTES_FROM_END, START, bytes - BYTES_FROM_END);
	}
	
	return (READ_INDEX + bytes) % CAPACITY;
}
