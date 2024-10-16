#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include "../../sharedUserKernel.h"
#define STDIN 0
#define STDOUT 1
#define STDERR 2
void * memset(void * destination, int32_t character, uint64_t length);

void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
int myStrcmp(const char s1, const char s2);
//freezes for the amount of ms indicated
void sleepms(uint32_t ms);
void reverseStr(char* str, int length);
void intToStr(uint64_t num, char* str);
void printMm();
void runTestMm();
void testeando();
void printNumber(uint64_t num,uint32_t hexColor);
void exitProcess(uint64_t status);
#endif