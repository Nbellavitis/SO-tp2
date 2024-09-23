// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "mm.h"
#include <stdint.h>
#include "../Drivers/include/videoDriver.h"
#define START_HEAP 0x600000


enum {FREE,USED, START_BOUNDARY,END_BOUNDARY, SINGLE_BLOCK};

typedef struct {
    uint32_t index;
    uint32_t blocks;
} lastFreed;

typedef struct MM{
   void * start;
   uint32_t blockQty;
   uint32_t blocksUsed;
   uint32_t *bitmap;
   uint32_t current;
   lastFreed lastFreed;
} mm;

static void initializeBitmap();
static size_t sizeToBlockQty(size_t size);

mm memoryManager;
int mmInit ( void * baseAddress ,uint64_t memorySize)  {
    size_t totalNeeded = memorySize;
    memoryManager.start = baseAddress;
    memoryManager.blockQty = totalNeeded / BLOCK_SIZE;
    if(memorySize % BLOCK_SIZE != 0){
        memoryManager.blockQty++;
        totalNeeded += BLOCK_SIZE * memoryManager.blockQty;
    }
    size_t bitMapSize = memoryManager.blockQty / BLOCK_SIZE;
    if(memoryManager.blockQty % BLOCK_SIZE != 0){
        bitMapSize++;
    }
    totalNeeded += bitMapSize * BLOCK_SIZE;
    if(totalNeeded > HEAP_SIZE){
        return -1;
    }

    memoryManager.bitmap = baseAddress;
    memoryManager.blocksUsed = 0;
    memoryManager.start = memoryManager.bitmap + bitMapSize * BLOCK_SIZE;
    memoryManager.current = 0;

    initializeBitmap();
    return 0;
}
static size_t sizeToBlockQty(size_t size){
    return (size_t) ((size % BLOCK_SIZE)? (size / BLOCK_SIZE) + 1 : size / BLOCK_SIZE);
}

static void initializeBitmap(){
    for(int i = 0; i < memoryManager.blockQty; i++){
        memoryManager.bitmap[i] = FREE;
    }
}
static uintptr_t findFreeBlocks( size_t blocksNeeded, size_t start, size_t end){
    size_t freeBlocks = 0;
    for(size_t i = start; i < end; i++){
        if(memoryManager.bitmap[i] == FREE){
            freeBlocks++;
            if(freeBlocks == blocksNeeded){
                return (uintptr_t)(memoryManager.start + (i-blocksNeeded+1) * BLOCK_SIZE);
            }
        }else{
            freeBlocks = 0;
        }
    }
    return -1;
}
static void * markGroupAsUsed(uint32_t blocksNeeded, uint32_t index){
    memoryManager.bitmap[index] = START_BOUNDARY;
    memoryManager.bitmap[index + blocksNeeded -1] = END_BOUNDARY;
    memoryManager.blocksUsed += 2;
    for(int i = 1; i < blocksNeeded -1; i++){
        memoryManager.blocksUsed++;
        memoryManager.bitmap[index + i] = USED;
    }
    return (void *) (memoryManager.start + index * BLOCK_SIZE);
}

void * allocMemory(size_t size){
    size_t blocksNeeded = sizeToBlockQty(size);
    if(blocksNeeded > memoryManager.blockQty - memoryManager.blocksUsed){
        return NULL;
    }
    if(blocksNeeded < memoryManager.lastFreed.blocks){
        return markGroupAsUsed(memoryManager.lastFreed.blocks, memoryManager.lastFreed.index);
        
    }
    uintptr_t initialBlockAddress = findFreeBlocks( blocksNeeded, memoryManager.current, memoryManager.blockQty);
    
    if(initialBlockAddress == -1){
        initialBlockAddress = findFreeBlocks( blocksNeeded, 0, memoryManager.current + blocksNeeded);
       
    }
     if(initialBlockAddress == -1){
        
         return NULL;
 }
    memoryManager.current = (initialBlockAddress) / BLOCK_SIZE + blocksNeeded;
    if(blocksNeeded == 1){
    
         memoryManager.blocksUsed++;
       
        memoryManager.bitmap[(initialBlockAddress - (uintptr_t) memoryManager.start) / BLOCK_SIZE] = SINGLE_BLOCK;
       
        return (void *) initialBlockAddress;
     }
     return markGroupAsUsed(blocksNeeded, (initialBlockAddress - (uintptr_t) memoryManager.start) / BLOCK_SIZE);
    
}

void freeMemory(void * memory){
    if(memory == NULL || (uintptr_t) memory % BLOCK_SIZE != 0){
        return;
    }
    uintptr_t blockAddress = (uintptr_t) memory;
    size_t blockIndex = (blockAddress - (uintptr_t) memoryManager.start) / BLOCK_SIZE;
    memoryManager.lastFreed.index = blockIndex;
    if(memoryManager.bitmap[blockIndex] == SINGLE_BLOCK){
        memoryManager.bitmap[blockIndex] = FREE;
        memoryManager.lastFreed.blocks = 1;
        memoryManager.blocksUsed--;
        return;
    }
    if(memoryManager.bitmap[blockIndex] != START_BOUNDARY){
        return;
    }
    size_t blocksToFree = 0;
    while(memoryManager.bitmap[blockIndex + blocksToFree] != END_BOUNDARY){
        memoryManager.bitmap[blockIndex + blocksToFree] = FREE;
        blocksToFree++;
    }
    memoryManager.bitmap[blockIndex + blocksToFree] = FREE;
    memoryManager.blocksUsed -= blocksToFree + 1;
    memoryManager.lastFreed.blocks = blocksToFree + 1;
}
MemoryStatus getMemoryStatus(){
    MemoryStatus status;
    status.totalMemory = memoryManager.blockQty * BLOCK_SIZE;
    status.usedMemory = memoryManager.blocksUsed * BLOCK_SIZE;
    status.freeMemory = status.totalMemory - status.usedMemory;
    return status;
}