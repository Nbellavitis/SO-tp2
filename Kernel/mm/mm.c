// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "mm.h"
#include <stdint.h>
#include "../include/lib.h"
#include "../Drivers/include/videoDriver.h"
#define START_HEAP 0x600000


enum {FREE,USED, START_BOUNDARY,END_BOUNDARY, SINGLE_BLOCK};

typedef struct {
    uint32_t index;
    uint32_t blocks;
} lastFreed;

typedef struct mm{
   void * start;
   uint32_t blockQty;
   uint32_t blocksUsed;
   uint32_t *bitmap;
   size_t current;
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
        totalNeeded = BLOCK_SIZE * memoryManager.blockQty;
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
    size_t i;
    for(i = start; i < end; i++){
        if(memoryManager.bitmap[i] == FREE){
            freeBlocks++;
            if(freeBlocks == blocksNeeded){
                return (uintptr_t)(memoryManager.start + (i-blocksNeeded+1) * BLOCK_SIZE);
            }
        }else{
            freeBlocks = 0;
        }
    }
    if(freeBlocks == blocksNeeded){
        return (uintptr_t)(memoryManager.start + (i-blocksNeeded+1) * BLOCK_SIZE);
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
     initialBlockAddress = findFreeBlocks( blocksNeeded, 0, memoryManager.blockQty);
    }
     if(initialBlockAddress == -1){
         return NULL;
 }
    memoryManager.current = sizeToBlockQty(initialBlockAddress - (uintptr_t) memoryManager.start) +blocksNeeded;

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
void fillWith1s(void * memory, size_t size){
    for(int i = 0; i < size; i++){
        ((char *) memory)[i] = '1';
    }

}
void printMemory() {
    char *aux = allocMemory(BLOCK_SIZE);
    fillWith1s(aux, BLOCK_SIZE);

    for (int i = 0; i < 5; i++){
        char * aux2=allocMemory(BLOCK_SIZE);
        printNumber(memoryManager.current,0x00ffffff);
        newLine();
        fillWith1s(aux2, BLOCK_SIZE);

    }


    for (int i = 0; i < BLOCK_SIZE * 50; i++) {
        drawChar(0x00ffffff, *(aux + i));
    }
}

void memoryManager_status() {
    uint64_t total = 0;
    uint64_t total2 = 0;
    uint64_t from = 0;
    uint64_t to = 0;

    uint64_t to2 = 0;

    uint64_t max_continous = 0;
    uint64_t start_continous = 0;

    int flag = 0;

    for (uint32_t i = 0; i < memoryManager.blockQty; i++) {
        if (memoryManager.bitmap[i] != FREE && memoryManager.bitmap[i] != USED   && memoryManager.bitmap[i] != END_BOUNDARY && memoryManager.bitmap[i] != START_BOUNDARY && memoryManager.bitmap[i] != SINGLE_BLOCK) {
            drawWord(0xFFFFFFFF," ACA HAY UN VALOR QUE NO ESTA BIEN: ");
            printNumber(memoryManager.bitmap[i],0xFFFFFFFF);
            drawWord(0xFFFFFFFF," en posicion: ");
            printNumber(i,0xFFFFFFFF);
            newLine();
            total++;
        }
        if (memoryManager.bitmap[i] == FREE ) {
            if (total2 == 0) {
                from = i;
            }
            total2++;
            to2 = i;
            if (max_continous == 0 && !flag) {
                start_continous = i;
                flag = 1;

            }
            max_continous++;
        } else if (max_continous != 0){

            max_continous = 0;
        }
    }
    drawWord(0xFFFFFFFF,"total total : ");
    printNumber(total2,0xFFFFFFFF);
    newLine();
    drawWord(0xFFFFFFFF,"from: ");
    printNumber(from,0xFFFFFFFF);
    newLine();
    drawWord(0xFFFFFFFF,"to: ");
    printNumber(to2,0xFFFFFFFF);
    newLine();
    drawWord(0xFFFFFFFF,"total with garbage : ");
    printNumber(total,0xFFFFFFFF);
    newLine();
    drawWord(0xFFFFFFFF,"en pos 0 hay: ");
    printNumber(memoryManager.bitmap[0],0xFFFFFFFF);
    newLine();


}