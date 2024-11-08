#ifdef BUDDY
#include "../Drivers/include/videoDriver.h"
#include "mm.h"
#include <stdint.h>
#define START_HEAP 0x600000

#define TWO_TO_THE(x) ((uint64_t)1 << (x))

#define MIN_ALLOC_LOG2 6 // 64B
#define MIN_ALLOC TWO_TO_THE(MIN_ALLOC_LOG2)

#define MAX_ALLOC_LOG2 26 // 256MB
#define MAX_ALLOC TWO_TO_THE(MAX_ALLOC_LOG2)

#define LEVEL_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)
#define NODES (TWO_TO_THE(MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1) - 1)

#define FREE 0
#define SPLIT 1
#define USED 2
MemoryStatus status;
typedef struct Block {
  uint8_t order;
  struct Block *next;
  uint8_t status;
} Block;

typedef struct Buddy {
  Block *freeList[LEVEL_COUNT];
  void *start;
  uint64_t totalMemorySize;
} Buddy;
Buddy buddy;

static uint64_t align(uint64_t value, uint64_t alignment) {
  return (value + alignment - 1) & ~(alignment - 1);
}
int mmInit(void *baseAddress, uint64_t memorySize) {
  size_t totalNeeded = align(memorySize, 1 << MIN_ALLOC_LOG2);
  if (totalNeeded > MAX_ALLOC || totalNeeded < MIN_ALLOC ||
      totalNeeded > HEAP_SIZE) {
    return -1;
  }
  buddy.start = baseAddress;
  for (int i = 0; i < LEVEL_COUNT; i++) {
    buddy.freeList[i] = NULL;
  }
  Block *firstBlock = (Block *)baseAddress;
  firstBlock->order = MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2;
  firstBlock->next = NULL;
  firstBlock->status = FREE;
  status.totalMemory = totalNeeded;
  status.usedMemory = 0;
  status.freeMemory = totalNeeded;
  buddy.freeList[MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2] = firstBlock;
  buddy.totalMemorySize = totalNeeded;
  return 0;
}

void *allocMemory(size_t size) {
  size = align(size, 1 << MIN_ALLOC_LOG2) + sizeof(Block);
  if (size > buddy.totalMemorySize) {
    return NULL;
  }

  int order = 0;
  while ((1 << (MIN_ALLOC_LOG2 + order)) < size) {
    order++;
  }
  if (order >= LEVEL_COUNT) {
    return NULL;
  }

  int index = order;
  while (index < LEVEL_COUNT && buddy.freeList[index] == NULL) {
    index++;
  }
  if (index >= LEVEL_COUNT) {
    return NULL;
  }

  Block *block = buddy.freeList[index];
  buddy.freeList[index] = block->next;
  while (index > order) {
    index--;
    Block *buddyBlock =
        (Block *)((uint64_t)block + (1 << (MIN_ALLOC_LOG2 + index)));
    buddyBlock->order = index;
    buddyBlock->status = FREE;
    buddyBlock->next = buddy.freeList[index];
    buddy.freeList[index] = buddyBlock;
  }
  block->status = USED;
  block->order = order;
  uint64_t blockSize = 1 << (MIN_ALLOC_LOG2 + order);
  status.usedMemory += blockSize;
  status.freeMemory -= blockSize;
  return (void *)((uint8_t *)block + sizeof(Block));
}

void freeMemory(void *address) {
  if (address == NULL || address < buddy.start ||
      address >= (void *)((uint64_t)buddy.start + buddy.totalMemorySize)) {
    return;
  }
  Block *block = (Block *)((uint8_t *)address - sizeof(Block));
  if (block->status != USED) {
    return;
  }

  uint64_t blockSize = 1 << (MIN_ALLOC_LOG2 + block->order);
  block->status = FREE;
  status.usedMemory -= blockSize;
  status.freeMemory += blockSize;
  int index = block->order;

  while (index < LEVEL_COUNT - 1) {
    uintptr_t buddyAddress = (uintptr_t)block ^ (1 << (MIN_ALLOC_LOG2 + index));
    if (buddyAddress < (uintptr_t)buddy.start ||
        buddyAddress >= (uintptr_t)buddy.start + buddy.totalMemorySize) {
      break;
    }
    Block *buddyBlock = (Block *)buddyAddress;
    if (buddyBlock->status != FREE || buddyBlock->order != block->order) {
      break;
    }
    Block **prev = &buddy.freeList[index];
    while (*prev && *prev != buddyBlock) {
      prev = &(*prev)->next;
    }
    if (*prev) {
      *prev = buddyBlock->next;
    }
    if (buddyBlock < block) {
      block = buddyBlock;
    }
    index++;
    block->order++;
  }
  block->next = buddy.freeList[index];
  buddy.freeList[index] = block;
}

MemoryStatus getMemoryStatus() { return status; }
#endif