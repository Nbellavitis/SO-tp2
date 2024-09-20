#ifndef MM_h
#define MM_h
#include <stdlib.h>
#include <stdint.h>
#define HEAP_SIZE 0x10000000 // 256 MiB
#define BLOCK_SIZE 64

typedef struct MemoryManagerCDT * MemoryManagerADT;
void mmInit ( void * baseAddress ,uint64_t memorySize) ;
#endif