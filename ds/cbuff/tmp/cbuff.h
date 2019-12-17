/*************************
    circular cbuff
      Header File
    Author : OL80
      Date : 03/12/2019           
*************************/

#ifndef C_BUUFER_H
#define C_BUUFER_H

typedef struct cbuff cbuff_t;

#include <sys/types.h> /* ssize_t */
#include <stddef.h> /* size_t */

/* - create a new cbuff with capacity of capacity ------------------------- */
/* - capacity must be greater then 0 ---------------------------------------- */
cbuff_t *CbuffCreate(size_t capacity);

/* - destroys an existing cbuff ------------------------------------------- */
/* - cbuff must not be NULL ----------------------------------------------- */
void CbuffDestroy(cbuff_t *cbuff);

/* - writes bytes bytes to the cbuff -------------------------------------- */
/* - the return is the number of bytes written ----------------------------- */
/* - cbuff must not be NULL ----------------------------------------------- */
/* - data must not be NULL ------------------------------------------------- */
ssize_t CbuffWrite(cbuff_t *cbuff, const void *data, size_t bytes);

/* - reads bytes bytes from the cbuff ------------------------------------- */
/* - return the number of bytes successfully read -------------------------- */
/* - cbuff must not be NULL ----------------------------------------------- */
/* - data must not be NULL ------------------------------------------------- */
ssize_t CbuffRead(cbuff_t *cbuff, void *data, size_t bytes);

/* - return 1 if the cbuff is empty and 0 if it is not -------------------- */
/* - cbuff must not be NULL ----------------------------------------------- */
int CbuffIsEmpty(const cbuff_t *cbuff);

/* - return the current free number of bytes in the cbuff ----------------- */
/* - cbuff must not be NULL ----------------------------------------------- */
size_t CbuffFreeSpace(const cbuff_t *cbuff);

/* - return the cbuff's maximum capacity ---------------------------------- */
/* - cbuff must not be NULL ----------------------------------------------- */
size_t CbuffCapacity(const cbuff_t *cbuff);

#endif /* C_BUUFER_H */
