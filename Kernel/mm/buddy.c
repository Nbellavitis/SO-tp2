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

#define MAX_NODE_POOL_SIZE 1024  // Adjust this size as needed

typedef enum {
    FREE,
    USED
} BlockState;

typedef struct Block {
    BlockState state;
    uint8_t size;
    struct Block *next;
} Block;

typedef struct Buddy {
    Block *free_list[LEVEL_COUNT];
    uint64_t max_level;
    uint64_t used_memory;
} Buddy;

static Buddy buddy;
static Block node_pool[NODE_POOL_SIZE];
static Block *free_node_list = NULL;

void initNodePool() {
    for (int i = 0; i < NODE_POOL_SIZE - 1; i++) {
        node_pool[i].next = &node_pool[i + 1];
    }
    node_pool[NODE_POOL_SIZE - 1].next = NULL;
    free_node_list = &node_pool[0];
}

Block *allocateNode() {
    if (free_node_list == NULL) {
        return NULL;  // No more nodes available
    }
    Block *node = free_node_list;
    free_node_list = free_node_list->next;
    return node;
}

void freeNode(Block *node) {
    node->next = free_node_list;
    free_node_list = node;
}

int mmInit(void *baseAddress, uint64_t memorySize) {
    if (memorySize < MIN_ALLOC || memorySize > MAX_ALLOC || (memorySize & (memorySize - 1)) != 0) {
        return -1; // Memory size must be a power of 2 and within the allowed range
    }

    initNodePool();

    for (int i = 0; i < LEVEL_COUNT; i++) {
        buddy.free_list[i] = NULL;
    }

    Block *initial_block = (Block *)START_HEAP;
    initial_block->state = FREE;
    initial_block->next = NULL;
    initial_block->buddy = NULL;
    buddy.free_list[LEVEL_COUNT - 1] = initial_block;

    return 0;
}

void *reserveMem(uint16_t level) {
    if (buddy.free_list[level] != NULL) {
        Block *block = buddy.free_list[level];
        buddy.free_list[level] = block->next;
        block->state = USED;
        return block;
    }
    return NULL;
}

uint8_t split(uint16_t level) {
    int counter = 0;
    while (buddy.free_list[level + counter] == NULL) {
        counter++;
        if (level + counter >= LEVEL_COUNT) {
            return -1;
        }
    }
    while (counter > 0) {
        Block *block = buddy.free_list[level + counter];
        buddy.free_list[level + counter] = block->next;

        Block *buddy1 = block;
        Block *buddy2 = (Block *)((uint8_t *)block + (1 << (level + counter - 1)));

        buddy1->state = FREE;
        buddy2->state = FREE;
        buddy1->next = buddy2;
        buddy2->next = buddy.free_list[level + counter - 1];
        buddy.free_list[level + counter - 1] = buddy1;

        counter--;
    }
    return 0;
}

void *malloc(uint64_t size) {
    if (size == 0 || size > MAX_ALLOC) {
        return NULL;
    }

    uint16_t level = findLevel(size);

    void *ptr = reserveMem(level);
    if (ptr != NULL) {
        return ptr;
    }

    if (split(level) != -1) {
        return reserveMem(level);
    }
    return NULL;
}