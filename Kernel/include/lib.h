#ifndef LIB_H
#define LIB_H

#include <stdint.h>
typedef int64_t pid_t;
#define STDIN 0
#define STDOUT 1
#define STDERR 2
void * memset(void * destination, int32_t character, uint64_t length);

void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

//freezes for the amount of ms indicated
void sleepms(int ms);
void reverseStr(char* str, int length);
void intToStr(int num, char* str);
void printMm();
void runTestMm();
#endif