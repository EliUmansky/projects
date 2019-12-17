/********************************************
*		Author: Igal Bogopolsky				*
*		Reviewer: Eli Umansky				*
*		Date: 4.12.2019					    *
*		Data Structures - Circular Buffer   *
********************************************/

#include <stdlib.h> /* malloc */
#include <assert.h>	/* assert */
#include <string.h>	/* memcpy */
#include <sys/types.h> /* ssize_t */

#include "cbuff.h"

#define byte char

struct cbuff
{
    byte* start;
    size_t capacity;
    size_t read_index;
    size_t size;
};

cbuff_t *CbuffCreate(size_t capacity)
{
    cbuff_t *buffer = NULL;

    assert(0 < capacity);

    buffer = (cbuff_t *)malloc(sizeof(cbuff_t) + capacity);
    if(NULL == buffer)
    {
        return NULL;
    }

    buffer->start = (byte *)buffer + sizeof(cbuff_t);
    buffer->capacity = capacity;
    buffer->read_index = 0;
    buffer->size = 0;

    return buffer;
}

void CbuffDestroy(cbuff_t *buffer)
{
    assert(NULL != buffer);

    free(buffer);
}

ssize_t CbuffWrite(cbuff_t *buffer, const void *data, size_t bytes)
{
	char *write_to = 0;
	size_t bytes_till_end = 0;
	size_t left_to_copy = 0;
	size_t free_bytes = 0;
	size_t write_start = 0;

	assert(NULL != buffer);
	assert(NULL != data);

	free_bytes = CbuffFreeSpace(buffer) < bytes ? CbuffFreeSpace(buffer) : bytes;
	write_start = (buffer->read_index + buffer->size) % buffer->capacity;
	write_to = (buffer->start + write_start);
	bytes_till_end = (buffer->start + buffer->capacity) - write_to;

	if(free_bytes <= bytes_till_end)
	{
		memcpy(write_to, data, free_bytes);
		buffer->size += free_bytes;

		return free_bytes;
	}

	memcpy(write_to, data, bytes_till_end);
	data = (byte*)data + bytes_till_end;
	left_to_copy = free_bytes - bytes_till_end;

	memcpy(buffer->start, data, left_to_copy);
	buffer->size += free_bytes;

	return free_bytes;
}

ssize_t CbuffRead(cbuff_t *buffer, void *dest, size_t bytes)
{
	size_t bytes_to_read = 0;
	size_t left_to_copy = 0;
	size_t bytes_till_end = 0;
	byte *read_start = 0;

	assert(NULL != buffer);
	assert(NULL != dest);
	
	read_start = buffer->start + buffer->read_index;
	bytes_to_read = buffer->size < bytes ? buffer->size : bytes;
	bytes_till_end = (buffer->start + buffer->capacity) - read_start;

	if(bytes_to_read <= bytes_till_end)
	{
		memcpy(dest, read_start, bytes_to_read);
		buffer->read_index = (buffer->read_index + bytes_to_read) % buffer->capacity;
		buffer->size -= bytes_to_read;

		return bytes_to_read;
	}
	
	memcpy(dest, read_start, bytes_till_end);
	dest = (byte*)dest + bytes_till_end;
	left_to_copy = bytes_to_read - bytes_till_end;

	memcpy(dest, buffer->start, left_to_copy);	
	buffer->read_index = (buffer->read_index + bytes_to_read) % buffer->capacity;
	buffer->size -= bytes_to_read;
	
	return bytes_to_read;
}

size_t CbuffCapacity(const cbuff_t *buffer)
{
    assert(NULL != buffer);

    return buffer->capacity;
}

size_t CbuffFreeSpace(const cbuff_t *buffer)
{
    assert(NULL != buffer);

    return (buffer->capacity - buffer->size);
}

int CbuffIsEmpty(const cbuff_t *buffer)
{
    return (buffer->size == 0);
}
