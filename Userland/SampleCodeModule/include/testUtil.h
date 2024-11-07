#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdint.h>
#include "usrSysCall.h"
uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussyWait(uint64_t n);
void endlessLoop();
int64_t testProcesses(uint64_t argc, char *argv[]);
void testPrio();
uint64_t testSync(uint64_t argc, char *argv[]);
#endif