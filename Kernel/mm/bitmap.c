// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#ifdef BITMAP
#include "../Drivers/include/videoDriver.h"
#include "../include/lib.h"
#include "mm.h"
#include <stdint.h>
#define START_HEAP 0x600000

enum { FREE, USED, START_BOUNDARY, END_BOUNDARY, SINGLE_BLOCK };

typedef struct mm {
  void *start;
  uint32_t blockQty;
  uint32_t blocksUsed;
  uint32_t *bitmap;
  uint32_t current;
} mm;

static void initializeBitmap();
static uint32_t sizeToBlockQty(uint32_t size);

mm memoryManager;
int mmInit(void *baseAddress, uint64_t memorySize) {
  uint32_t totalNeeded = memorySize;
  memoryManager.start = baseAddress;
  memoryManager.blockQty = totalNeeded / BLOCK_SIZE;
  if (memorySize % BLOCK_SIZE != 0) {
    memoryManager.blockQty++;
    totalNeeded = BLOCK_SIZE * memoryManager.blockQty;
  }
  uint32_t bitMapSize = memoryManager.blockQty / BLOCK_SIZE;
  if (memoryManager.blockQty % BLOCK_SIZE != 0) {
    bitMapSize++;
  }
  totalNeeded += bitMapSize * BLOCK_SIZE;
  if (totalNeeded > HEAP_SIZE) {
    return -1;
  }

  memoryManager.bitmap = baseAddress;
  memoryManager.blocksUsed = 0;
  memoryManager.start = memoryManager.bitmap + bitMapSize * BLOCK_SIZE;
  memoryManager.current = 0;

  initializeBitmap();
  return 0;
}
static uint32_t sizeToBlockQty(uint32_t size) {
  return (uint32_t)((size % BLOCK_SIZE) ? (size / BLOCK_SIZE) + 1
                                        : size / BLOCK_SIZE);
}

static void initializeBitmap() {
  for (int i = 0; i < memoryManager.blockQty; i++) {
    memoryManager.bitmap[i] = FREE;
  }
}
static uintptr_t findFreeBlocks(uint32_t blocksNeeded, uint32_t start,
                                uint32_t end) {
  uint32_t freeBlocks = 0;
  uint32_t i;
  for (i = start; i < end; i++) {
    if (memoryManager.bitmap[i] == FREE) {
      freeBlocks++;
      if (freeBlocks == blocksNeeded) {
        return (uintptr_t)(memoryManager.start +
                           (i - blocksNeeded + 1) * BLOCK_SIZE);
      }
    } else {
      freeBlocks = 0;
    }
  }
  if (freeBlocks == blocksNeeded) {
    return (uintptr_t)(memoryManager.start +
                       (i - blocksNeeded + 1) * BLOCK_SIZE);
  }
  return 0;
}
static void *markGroupAsUsed(uint32_t blocksNeeded, uint32_t index) {
  memoryManager.bitmap[index] = START_BOUNDARY;
  memoryManager.bitmap[index + blocksNeeded - 1] = END_BOUNDARY;
  memoryManager.blocksUsed += 2;
  for (int i = 1; i < blocksNeeded - 1; i++) {
    memoryManager.blocksUsed++;
    memoryManager.bitmap[index + i] = USED;
  }
  return (void *)(memoryManager.start + index * BLOCK_SIZE);
}

void *allocMemory(size_t size) {
  uint32_t blocksNeeded = sizeToBlockQty(size);
  if (blocksNeeded > memoryManager.blockQty - memoryManager.blocksUsed) {
    drawWord(0xFF0000, "No hay suficiente memoria");
    return NULL;
  }
  uintptr_t initialBlockAddress = findFreeBlocks(
      blocksNeeded, memoryManager.current, memoryManager.blockQty);

  if (initialBlockAddress == 0) {
    initialBlockAddress =
        findFreeBlocks(blocksNeeded, 0, memoryManager.blockQty);
  }
  if (initialBlockAddress == 0) {
    drawWord(0xFF0000, "No hay suficiente memoria");
    return NULL;
  }
  memoryManager.current =
      sizeToBlockQty(initialBlockAddress - (uintptr_t)memoryManager.start) +
      blocksNeeded;
  if (blocksNeeded == 1) {

    memoryManager.blocksUsed++;

    memoryManager
        .bitmap[(initialBlockAddress - (uintptr_t)memoryManager.start) /
                BLOCK_SIZE] = SINGLE_BLOCK;

    return (void *)initialBlockAddress;
  }
  return markGroupAsUsed(
      blocksNeeded,
      (initialBlockAddress - (uintptr_t)memoryManager.start) / BLOCK_SIZE);
}

void freeMemory(void *memory) {
  if (memory == NULL || (uintptr_t)memory % BLOCK_SIZE != 0) {
    return;
  }
  uintptr_t blockAddress = (uintptr_t)memory;
  uint32_t blockIndex =
      (blockAddress - (uintptr_t)memoryManager.start) / BLOCK_SIZE;
  if (memoryManager.bitmap[blockIndex] == SINGLE_BLOCK) {
    memoryManager.bitmap[blockIndex] = FREE;
    memoryManager.blocksUsed--;
    return;
  }
  if (memoryManager.bitmap[blockIndex] != START_BOUNDARY) {
    return;
  }
  uint32_t blocksToFree = 0;
  while (memoryManager.bitmap[blockIndex + blocksToFree] != END_BOUNDARY) {
    memoryManager.bitmap[blockIndex + blocksToFree] = FREE;
    blocksToFree++;
  }
  memoryManager.bitmap[blockIndex + blocksToFree] = FREE;
  memoryManager.blocksUsed -= blocksToFree + 1;
}
MemoryStatus getMemoryStatus() {
  MemoryStatus status;
  status.totalMemory = memoryManager.blockQty * BLOCK_SIZE;
  status.usedMemory = memoryManager.blocksUsed * BLOCK_SIZE;
  status.freeMemory = status.totalMemory - status.usedMemory;
  return status;
}
#endif