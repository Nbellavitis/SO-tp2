#include "../include/sems.h"
#include "../Drivers/include/videoDriver.h"
#define SEMAPHORES_CAPACITY 10
typedef struct semData {
    uint64_t sem;
    const char *name;
    int isLinked;
    uint64_t value;
    uint8_t lock;
    uint64_t attachedProcesses;
    queueADT waitingQueue;
} semdata_t;

typedef struct semCollectionCDT {
    semdata_t *semaphores[SEMAPHORES_CAPACITY];
    uint64_t semaphoresCount;
} semCollectionCDT;
semCollectionCDT * semCollection;
int semInit(){
   semCollection = (semCollectionCDT * ) allocMemory(sizeof(semCollectionCDT));
  if(semCollection == NULL){
    return -1;
  }
  for(int i = 0; i < SEMAPHORES_CAPACITY; i++){
        semCollection->semaphores[i] = NULL;
  }
  return 0;
}

static int lookupSemaphore(const char *name){
  for(int i = 0; i < SEMAPHORES_CAPACITY; i++){
    if(semCollection->semaphores[i] != NULL && strcmp(semCollection->semaphores[i]->name, name) == 0){
      return i;
    }
  }
  return -1;
}
static uint8_t getSemLock(semCollectionCDT * semCollection, semdata_t * sem){
  return sem->lock;
}
int semOpen(char *name, int initialValue){
 int aux= lookupSemaphore(name);
 if(aux != -1){
    return 1;
  }
  for(int i = 0; i < SEMAPHORES_CAPACITY; i++){
    if(semCollection->semaphores[i] == NULL){
      semdata_t * sem = (semdata_t *) allocMemory(sizeof(semdata_t));
      if(sem == NULL){
        return 0;
      }
      sem->sem = i;
      sem->name = name;
      sem->isLinked = 1;
      sem->value = initialValue;
      sem->attachedProcesses = 0;
      sem->waitingQueue = createQueue(comparePCB);
      semCollection->semaphores[i] = sem;
      sem->lock = (initialValue > 0) ? 1 : 0;
      semCollection->semaphoresCount++;
      return 1;
    }
  }
  return 0;
}
void semPost(char *name){
  int aux = lookupSemaphore(name);
  if(aux == -1){
    return;
  }

 semdata_t * sem = semCollection->semaphores[aux];
  uint8_t semLock = getSemLock(semCollection, sem);
 acquire(&semLock);
  if(sem->attachedProcesses > 0){
    PCB aux = dequeue(sem->waitingQueue);
    sem->attachedProcesses--;
    unblockProcess(aux->pid);
    release(&semLock);
    return;
}
  sem->value++;
  release(&semLock);
 return;
}
void semWait(char *name){
  int aux = lookupSemaphore(name);
  if(aux == -1){
    return;
  }
  semdata_t * sem = semCollection->semaphores[aux];
  uint8_t semLock = getSemLock(semCollection, sem);
  acquire(&semLock);
  if(sem->value > 0){
    sem->value--;
    release(&semLock);
    return;
  }
  sem->attachedProcesses++;
  queue(sem->waitingQueue, lookUpOnHashMap(getActivePid()));
  release(&semLock);
  blockProcess(getActivePid());
  return;
}
void semClose(char *name){
  int aux = lookupSemaphore(name);
  if(aux == -1){
    return;
  }
  semdata_t * sem = semCollection->semaphores[aux];
  uint8_t semLock = getSemLock(semCollection, sem);
  acquire(&semLock);
  sem->attachedProcesses--;
  if(sizeQ(sem->waitingQueue) > 0){
    release(&semLock);
    return;
  }
  freeQueue(sem->waitingQueue);
  freeMemory(sem);
  semCollection->semaphores[aux] = NULL;
  semCollection->semaphoresCount--;
  release(&semLock);
  return;
}