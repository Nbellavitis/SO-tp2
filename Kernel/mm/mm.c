#include "mm.h"
#define BLOCK_SIZE 4096
#define BLOCK_COUNT 20
struct {
int blocksUsed;
void * start;
}usedMemory;

struct {
   void * start;
   uint32_t size;
   usedMemory free[BLOCK_COUNT];
   int current;
}MemoryManagerCDT;


MemoryManagerADT createMemoryManager( void * const restrict memoryForMemomyManager, void * const restrict managedMemory ) {
    MemoryManagerADT memoryManager = ( MemoryManagerADT ) memoryForMemomyManager;
    mmInit(memoryManager);
    return memoryManager;
}
void mmInit ( MemoryManagerADT memoryManager,void * managedMemory ) {
    memoryManager->start = managedMemory;
    memoryManager->size = BLOCK_SIZE;
    for(int i=0;i<BLOCK_COUNT;i++){
       memoryManager->free[i].start; = managedMemory + memoryManager->size * i
    }
   memoryManager->current = 0;
}

void * allocMemory(MemoryManagerADT const restrict memoryManager,const size_t memoryToAllocate) {
    int blocksToAllocate = (int) ceil(memoryToAllocate/BLOCK_SIZE); 
    
    if ( memoryManager->current + aux < BLOCK_COUNT ) {
        int aux =  memoryManager->current;
        memoryManager->current += blocksToAllocate;
      memoryManager->free[aux].blocksUsed=blocksToAllocate;
      return memoryManager->free[aux].start;
    } else {
        return NULL;
    }
}

void myFree(MemoryManagerADT const restrict memoryManager,void * const restrict memoryToFree ) {
     int blocksToFree = (int) ceil(memoryToFree/BLOCK_SIZE);
      for(int i = 0; i <= memoryManager->current;i++){
        if(memoryManager->free[i].start == memoryToFree){
              memoryManager->current-=memoryManager->free[i].blocksUsed;
              memoryManager->free[current].start=memoryToFree; 
        }
      }
}