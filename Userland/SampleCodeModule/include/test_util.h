#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#include <stdint.h>

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussy_wait(uint64_t n);
void endless_loop();
int64_t test_processes(uint64_t argc, char *argv[]);
void test_prio();
#endif