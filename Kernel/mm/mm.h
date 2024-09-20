#ifndef MM_h
#define MM_h
#include <stdlib.h>
#define HEAP_SIZE 0x10000000 // 256 MiB
#define BLOCK_SIZE 64

typedef struct MemoryManagerCDT * MemoryManagerADT;
MemoryManagerADT createMemoryManager(void * const restrict memoryForMemomyManager, void * const restrict managedMemory);
void * allocMemory(MemoryManagerADT const restrict memoryManager,const size_t memoryToAllocate);
void myFree(MemoryManagerADT const restrict memoryManager,void * const restrict memoryToFree );
#endif