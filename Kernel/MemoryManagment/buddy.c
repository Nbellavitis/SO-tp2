#include "mm.h"
#include <stdint.h>
#include "../Drivers/include/videoDriver.h"
#define START_HEAP 0x600000

#define TWO_TO_THE(x) ((uint64_t)1 << (x))

#define MIN_ALLOC_LOG2 6    // 64B
#define MIN_ALLOC TWO_TO_THE(MIN_ALLOC_LOG2)

#define MAX_ALLOC_LOG2 28   // 256MB
#define MAX_ALLOC TWO_TO_THE(MAX_ALLOC_LOG2)

#define LEVEL_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)

typedef enum {
    FREE,    
    USED    
} BlockState;

typedef struct Block{
    BlockState state;
    Block *next;
}Block;

typedef struct BuddySystem{
    Block * free_list[LEVEL_COUNT];
    size_t min_block_size;
    size_t max_block_size;
}BuddySystem;

static BuddySystem * buddy = NULL;

void initBuddy() {
    BuddySystem* buddy = (BuddySystem*) START_HEAP; 
    for (int i = 0; i < LEVEL_COUNT; i++) {
        buddy->free_list[i] = NULL;
    }
    Block* initial_block = (Block*)((uint8_t*)START_HEAP + sizeof(BuddySystem));
    initial_block->state = FREE;
    initial_block->next = NULL;
    buddy->free_list[LEVEL_COUNT - 1] = initial_block;
    buddy->min_block_size = MIN_ALLOC;
    buddy->max_block_size = MAX_ALLOC;
}

size_t findLevel(size_t size) {
    size_t level = 0;
    size_t mem = MIN_ALLOC;

    while(mem < size) {
        mem *= 2;
        level++;
    }
    return level;
}


void * reserveMem(uint16_t level) {
    if(buddy->free_list[level] != NULL){
        Block *block = buddy->free_list[level];
        buddy->free_list[level] = block->next;
        block->state = USED;
        return block;  
    }
    return NULL;
}

uint8_t split(uint16_t level){

    int counter = 0;
    while(buddy->free_list[level + counter] == NULL){
        counter++;
        if (level + counter >= LEVEL_COUNT) {
            return -1;  
        }
    }
    while(counter > 0){
        Block *block = buddy->free_list[level + counter];
        buddy->free_list[level + counter] = block->next;

        Block *buddy1 = block;
        Block *buddy2 = (Block*)((uint8_t*)block + (1 << (level + counter - 1)));

        buddy1->state = FREE;
        buddy2->state = FREE;
        buddy1->next = buddy2;
        buddy2->next = buddy->free_list[level + counter - 1];
        buddy->free_list[level + counter - 1] = buddy1;

        counter--;
    }
    
}



void * malloc(uint64_t size){
    if(size==0 || size > MAX_ALLOC){
        return NULL;
    } 

    if( buddy == NULL){
        initBuddy();
    }
    uint16_t level = findLevel(size);

    void * ptr = reserveMem(level);
    if(ptr != NULL){
        return ptr;
    }

    if(split(level) != -1){
        return reserveMem(level);
    }
    return NULL;
}