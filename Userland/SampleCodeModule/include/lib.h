#ifndef TPE_LIB_H
#define TPE_LIB_H
#include "../../../sharedUserKernel.h"
#include "testUtil.h"
#include <stddef.h>
#include <stdint.h>
#define WHITE 0xFFFFFFFF

int getC(char *c);
int strncmp(const char *str1, const char *str2, size_t n);
void putC(char c, uint32_t hexColor);
void putInt(int64_t num, uint32_t hexColor);
void print(uint32_t hexColor, const char *format, ...);
int strcmp(char *str1, char *str2);
char *cutString(char *str);
int strlen(const char *str);
void putString(const char *s, uint32_t hexColor);
int strToInt(char *s);
int getBuffer(char *buf);
void strToUpper(char *str);
void memset(void *destination, int32_t c, uint64_t length);
void intToStr(int64_t num, char *str);
void strAppend(char *str1, char *str2);
void freeProcessInfo(processInfoPtr *processes);
void printAllProcesses(processInfoPtr *processes);
void printProcess(processInfoPtr process);
char *strcpy(char *dest, const char *src);
char *strtok(char *str, const char *delim);
char *strchr(const char *str, int c);
char *strstr(const char *haystack, const char *needle);
#endif // TPE_LIB_H
