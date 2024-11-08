#ifndef LIB_H
#define LIB_H
#include <stddef.h>
#include <stdint.h>

#include "../../sharedUserKernel.h"
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define CHILD_PROCESS 1
#define READ_STDIN 2
#define SEM 3
void * memset(void * destination, int32_t character, uint64_t length);

void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
int strcmp(const char * s1, const char * s2);
//freezes for the amount of ms indicated
void sleepms(uint32_t ms);
void reverseStr(char* str, int length);
void intToStr(int64_t num, char* str);
void printMm();
void runTestMm();
void testeando();
void printNumber(int64_t num,uint32_t hexColor);
void exitProcess(uint64_t status);
int strcmp(const char *s1, const char *s2);
char * strAppend( char * s1,  char * s2);
size_t strlen(const char * s);
char * strcpy(char * destination, const char * source);
char * strcat(char * destination, const char * source);
#endif