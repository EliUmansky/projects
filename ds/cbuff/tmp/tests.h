/***************************
   Generic Tests File
      Header File
   Author : Greg Markovsky
     Date : 26/11/2019            
***************************/

#ifndef OL80_GREG_TESTS_H
#define OL80_GREG_TESTS_H

#include <stddef.h> /* size_t */
#include <sys/types.h> /* ssize_t */

#define NEW_LINE printf("\n");
#define PRINT_COLOR_SET_TO_RED printf("\x1b[31m");
#define PRINT_COLOR_SET_TO_GREEN printf("\x1b[32m");
#define PRINT_COLOR_RESET printf("\x1b[0m");
#define VERBOSE (1)
#define SILENT (0)
#define RUN_TEST(voc,test) test(voc); TestRep();

void TestStart(const char *name, const char *text);
void TestRep();
void TestInt(int voc_status, const char *str, int exp, int rec);
void TestSizeT(int voc_status, const char *str, size_t exp, size_t rec);
void TestSSizeT(int voc_status, const char *str, ssize_t exp, ssize_t rec);
void TestSizeTHex(int voc_status, const char *str, size_t exp, size_t rec);
void TestNotNull(int voc_status, const char *str, void *rec);
void TestBytes(int voc_status, const char *str, 
				const char *exp, const char *rec, size_t bytes);


#endif /* OL80_GREG_TESTS_H */
