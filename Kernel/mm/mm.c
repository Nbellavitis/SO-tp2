#include "mm.h"
#include <stdint.h>
#define START_HEAP 0x600000
//struct {
//int blocksUsed;
//void * start;
//}usedMemory;

enum {FREE,USED, START_BOUNDARY,END_BOUNDARY};

typedef struct MemoryManager {
   void * start;
   uint32_t blockQty;
   uint32_t blocksUsed;
   uint8_t *bitmap;
   uint32_t current;
}MemoryManager;

static void initializeBitmap();
MemoryManager memoryManager;
void mmInit ( void * baseAddress ,uint64_t memorySize)  {
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
        return;
    }

    memoryManager.bitmap = baseAddress;
    memoryManager.blocksUsed = 0;
    memoryManager.start = memoryManager.bitmap + bitMapSize * BLOCK_SIZE;
    memoryManager.current = 0;

    initializeBitmap(memoryManager);
}
static size_t sizeToBlockQty(size_t size){
    return (size % BLOCK_SIZE)? (size / BLOCK_SIZE) + 1 : size / BLOCK_SIZE;
}

static void initializeBitmap( ){
    for(int i = 0; i < memoryManager.blockQty; i++){
        memoryManager.bitmap[i] = FREE;
    }
}
static uintptr_t findFreeBlocks( size_t blocksNeeded, int start, int end){
    size_t freeBlocks = 0;
    for(int i = start; i < end; i++){
        if(memoryManager.bitmap[i] == FREE){
            freeBlocks++;
            if(freeBlocks == blocksNeeded){
                return (uintptr_t)(memoryManager.start + (i-blocksNeeded) * BLOCK_SIZE);
            }
        }else{
            freeBlocks = 0;
        }
    }
    return -1;
}

void * malloc(size_t size){
    size_t blocksNeeded = sizeToBlockQty(size);
    uintptr_t initialBlockAddress = findFreeBlocks( blocksNeeded, memoryManager.current, memoryManager.blockQty);
    if(initialBlockAddress == -1){
        initialBlockAddress = findFreeBlocks( blocksNeeded, 0, memoryManager.current + blocksNeeded);
    }
    if(initialBlockAddress == -1){
        return NULL;
    }
    if(blocksNeeded == 1){
        memoryManager.blocksUsed++;
        memoryManager.bitmap[(initialBlockAddress - (uintptr_t) memoryManager.start) / BLOCK_SIZE] = USED;
        return (void *) initialBlockAddress;
    }
    memoryManager.bitmap[(initialBlockAddress - (uintptr_t) memoryManager.start) / BLOCK_SIZE] = START_BOUNDARY;
    memoryManager.bitmap[(initialBlockAddress - (uintptr_t) memoryManager.start) / BLOCK_SIZE + blocksNeeded - 1] = END_BOUNDARY;
    for(int i = 1; i < blocksNeeded -1; i++){
        memoryManager.blocksUsed++;
        memoryManager.bitmap[(initialBlockAddress - (uintptr_t) memoryManager.start) / BLOCK_SIZE + i] = USED;
    }
    memoryManager.blocksUsed += 2;
    return (void *) initialBlockAddress;
}

void free(void * memory){
    uintptr_t blockAddress = (uintptr_t) memory;
    size_t blockIndex = (blockAddress - (uintptr_t) memoryManager.start) / BLOCK_SIZE;
    if(memoryManager.bitmap[blockIndex] != START_BOUNDARY){
        memoryManager.bitmap[blockIndex] = FREE;
        return;
    }
    size_t blocksToFree = 0;
    while(memoryManager.bitmap[blockIndex + blocksToFree] != END_BOUNDARY){
        memoryManager.bitmap[blockIndex + blocksToFree] = FREE;
        blocksToFree++;
    }
    memoryManager.bitmap[blockIndex + blocksToFree] = FREE;

}





