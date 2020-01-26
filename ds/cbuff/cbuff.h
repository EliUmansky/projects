/*************************
    circular buffer
      Header File
    Author : OL80
      Date : 03/12/2019           
*************************/

#ifndef C_BUUFER_H
#define C_BUUFER_H

typedef struct cbuff cbuff_t;

#include <stddef.h> /* size_t */
#include <sys/types.h> /* ssize_t */

/* - create a new buffer with capacity of capacity ------------------------- */
/* - capacity must be greater then 0 ---------------------------------------- */
cbuff_t *CbuffCreate(size_t capacity);

/* - destroys an existing buffer ------------------------------------------- */
/* - buffer must not be NULL ----------------------------------------------- */
void CbuffDestroy(cbuff_t *buffer);

/* - writes bytes bytes to the buffer -------------------------------------- */
/* - the return is the number of bytes written ----------------------------- */
/* - buffer must not be NULL ----------------------------------------------- */
/* - data must not be NULL ------------------------------------------------- */
ssize_t CbuffWrite(cbuff_t *buffer, const void *data, size_t bytes);

/* - reads bytes bytes from the buffer ------------------------------------- */
/* - return the number of bytes successfully read -------------------------- */
/* - buffer must not be NULL ----------------------------------------------- */
/* - data must not be NULL ------------------------------------------------- */
/* - bytes must be more then 0 --------------------------------------------- */
ssize_t CbuffRead(cbuff_t *buffer, void *data, size_t bytes);

/* - return 1 if the buffer is empty and 0 if it is not -------------------- */
/* - buffer must not be NULL ----------------------------------------------- */
int CbuffIsEmpty(const cbuff_t *buffer);

/* - return the current free number of bytes in the buffer ----------------- */
/* - buffer must not be NULL ----------------------------------------------- */
size_t CbuffFreeSpace(const cbuff_t *buffer);

/* - return the buffer's maximum capacity ---------------------------------- */
/* - buffer must not be NULL ----------------------------------------------- */
size_t CbuffCapacity(const cbuff_t *buffer);

#endif /* C_BUUFER_H */
