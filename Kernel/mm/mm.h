#ifndef MM_h
#define MM_h
#include <stdlib.h>
#include <stdint.h>
#define HEAP_SIZE 0x10000000 // 256 MiB
#define BLOCK_SIZE 64

#ifdef MM_BUDDY
#undef  HEAP_STRUCTURE_SIZE
#define HEAP_STRUCTURE_SIZE 0x800000
#endif // MM_BUDDY

typedef struct MemoryStatus {
    uint64_t totalMemory;
    uint64_t usedMemory;
    uint64_t freeMemory;
} MemoryStatus;

int mmInit ( void * baseAddress ,uint64_t memorySize) ;
void freeMemory(void * memory);
void * allocMemory(size_t size);
void printMemory();
MemoryStatus getMemoryStatus();
void memoryManager_status();
#endif