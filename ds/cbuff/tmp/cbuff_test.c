/****************************
     circular buffer
        Test File
    Author : Greg Markovsky
  Reviewer : Daniel Oren
      Date : 03/12/2019              
****************************/

#include <sys/types.h> /* ssize_t */
#include <stdio.h> /* printf */

#include "tests.h" /* TestStart */
#include "cbuff.h" /* cbuff_t */

#define BAD_ADDRESS (14)

static void TestNewCBuffer(int voc);
static void TestWriteBlind(int voc);
static void TestReadWriteBasic(int voc);
static void TestReadWriteAdvanced(int voc);

int main()
{
	/* pick SILENT or VERBOSE to run test as silent or verbose */

	RUN_TEST( VERBOSE , TestNewCBuffer)
	RUN_TEST( VERBOSE , TestWriteBlind)
	RUN_TEST( VERBOSE , TestReadWriteBasic)
	RUN_TEST( VERBOSE , TestReadWriteAdvanced)

	return 0;
}
/* -------------------------------------------- Test New Circular Buffer --- */

static void TestNewCBuffer(int voc)
{
	cbuff_t *buffer = NULL;
	const size_t capacity = 32;

	TestStart("New Buffer", 
	"create buffer, is empty, free space, capacity, destroy");

	buffer = CbuffCreate(capacity);

	TestNotNull(voc, "Create - new buffer", buffer);
	TestInt(voc, "IsEmpty - new buffer", 1, CbuffIsEmpty(buffer));
	TestSizeT(voc, "FreeSpace - new buffer", capacity, CbuffFreeSpace(buffer));
	TestSizeT(voc, "Capacity - new buffer", capacity, CbuffCapacity(buffer));	

	CbuffDestroy(buffer);
}

/* ---------------------------------------------------- Test Write Blind --- */

static void TestWriteBlind(int voc)
{
	cbuff_t *buffer = NULL;
	const size_t capacity = 32;
	const size_t wbytes = 20;
	const char *data = "abcdefghijklmnopqrstuvwxyz";
	ssize_t write_return = 0;

	TestStart("Write blind", "write , is empty, free space");

	buffer = CbuffCreate(capacity);
	write_return = CbuffWrite(buffer, data, wbytes);
	TestSSizeT(voc, "Write - within size", wbytes, write_return);
	TestInt(voc, "IsEmpty - partially full", 0, CbuffIsEmpty(buffer));
	TestSizeT(voc, "FreeSpace - partially full", 
		(capacity - wbytes), CbuffFreeSpace(buffer));

	write_return = CbuffWrite(buffer, data, wbytes);
	TestSSizeT(voc, "Write - beyond size", 12, write_return);
	TestInt(voc, "IsEmpty - full buffer", 0, CbuffIsEmpty(buffer));
	TestSizeT(voc, "FreeSpace - full buffer", 0, CbuffFreeSpace(buffer));

	CbuffDestroy(buffer);
}

/* ----------------------------------------------- Test Read Write Basic --- */

static void TestReadWriteBasic(int voc)
{
	cbuff_t *buffer = NULL;
	const size_t capacity = 32;
	const char *data = "abcdefghijklmnopqrst";
	char read_data[33] = { 0 };
	ssize_t write_return = 0;
	ssize_t read_return = 0;

	TestStart("Read Write Basic", 
		"read/write within size, beyond size, over gap");

	buffer = CbuffCreate(capacity);
	write_return = CbuffWrite(buffer, data, 20);

	read_return = CbuffRead(buffer, read_data, 10);
	TestSSizeT(voc, "Read - within size", 10, read_return);
	TestBytes(voc, "Read data - within size", "abcdefghij", read_data, 10);

	read_return = CbuffRead(buffer, read_data, 20);
	TestSSizeT(voc, "Read - beyond size", 10, read_return);
	TestBytes(voc, "Read data - beyond size", "klmnopqrst", read_data, 10);

	write_return = CbuffWrite(buffer, data, 20);
	TestSSizeT(voc, "Write - over gap, within size", 20, write_return);
	read_return = CbuffRead(buffer, read_data, 20);
	TestSSizeT(voc, "Read - over gap, exact size", 20, read_return);
	TestBytes(voc, "Read data - over gap, exact size", 
		"abcdefghijklmnopqrst", read_data, 20);

	CbuffDestroy(buffer);
}

/* -------------------------------------------- Test Read Write Advanced --- */

static void TestReadWriteAdvanced(int voc)
{
	cbuff_t *buffer = NULL;
	const size_t capacity = 32;
	const char *data = "abcdefghijklmnopqrstuvwxyzabcdef";
	char read_data[33] = { 0 };
	ssize_t write_return = 0;
	ssize_t read_return = 0;

	TestStart("Read Write Advanced", 
		"read/write over capacity, zero byte, empty, full");

	buffer = CbuffCreate(capacity);
	write_return = CbuffWrite(buffer, data, 20);
	read_return = CbuffRead(buffer, read_data, 20);
	write_return = CbuffWrite(buffer, data, 50);
	TestSSizeT(voc, "Write - over gap, over capacity", 32, write_return);
	read_return = CbuffRead(buffer, read_data, 50);
	TestSSizeT(voc, "Read - over gap, over capacity", 32, read_return);
	TestBytes(voc, "Read data - over gap, over capacity", 
		"abcdefghijklmnopqrstuvwxyzabcdef", read_data, 32);
	CbuffDestroy(buffer);

	buffer = CbuffCreate(capacity);
	write_return = CbuffWrite(buffer, data, 0);
	TestSSizeT(voc, "Write - write zero", 0, write_return);
	read_return = CbuffRead(buffer, read_data, 0);
	TestSSizeT(voc, "Read - read zero", 0, read_return);
	CbuffDestroy(buffer);

	buffer = CbuffCreate(capacity);
	read_return = CbuffRead(buffer, read_data, 10);
	TestSSizeT(voc, "Read - read from empty", 0, read_return);
	write_return = CbuffWrite(buffer, data, 32);
	write_return = CbuffWrite(buffer, data, 10);
	TestSSizeT(voc, "Write - write to full", 0, write_return);
	CbuffDestroy(buffer);
}



