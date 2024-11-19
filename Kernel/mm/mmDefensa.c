#include "mm.h"
typedef struct Stack{
  void * start;
  uint64_t size;
  void ** array;
  int current;
}Stack;
Stack stack;
int mmInit ( void * baseAddress ,uint64_t memorySize) {
  if(memorySize > HEAP_SIZE){
    return -1;
  }
  stack.array = baseAddress;
  stack.size = memorySize % BLOCK_SIZE ? memorySize / BLOCK_SIZE + 1 : memorySize / BLOCK_SIZE;
  stack.start = baseAddress + stack.size;
  stack.current = 0;
  for(int i = 0 ; i< stack.size ; i++){
    stack.array[i]=stack.start+i*BLOCK_SIZE;
  }
  return 0;
}
void * allocMemory(size_t size){
  if(stack.current >= stack.size){
    return NULL;
  }
  return stack.array[stack.current++];
}
void freeMemory(void * memory){

  if (!((memory - stack.start) % BLOCK_SIZE) || stack.current == 0){
    return;
  }
  stack.array[--stack.current] = memory;
}
MemoryStatus getMemoryStatus(){
   MemoryStatus status;
   status.totalMemory = stack.size * BLOCK_SIZE;
   status.usedMemory = stack.current * BLOCK_SIZE;
   status.freeMemory = status.totalMemory - status.usedMemory;
   return status;
}

