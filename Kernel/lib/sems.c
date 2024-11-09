// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "../include/sems.h"
#include "../Drivers/include/videoDriver.h"

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
semCollectionCDT *semCollection;
int semInit() {
  semCollection = (semCollectionCDT *)allocMemory(sizeof(semCollectionCDT));
  if (semCollection == NULL) {
    return -1;
  }
  for (int i = 0; i < SEMAPHORES_CAPACITY; i++) {
    semCollection->semaphores[i] = NULL;
  }
  return 0;
}

static int lookupSemaphore(const char *name) {
  for (int i = 0; i < SEMAPHORES_CAPACITY; i++) {
    if (semCollection->semaphores[i] != NULL &&
        strcmp(semCollection->semaphores[i]->name, name) == 0) {
      return i;
    }
  }
  return -1;
}
static uint8_t *getSemLock(semCollectionCDT *semCollection, semdata_t *sem) {
  return &(sem->lock);
}
int semOpen(char *name, int initialValue) {
  int aux = lookupSemaphore(name);
  if (aux != -1) {
    acquire(&(semCollection->semaphores[aux]->lock));
    if (semCollection->semaphores[aux] == NULL) {
      return 0;
    }
    semCollection->semaphores[aux]->attachedProcesses++;
    release(&(semCollection->semaphores[aux]->lock));
    return 1;
  }
  for (int i = 0; i < SEMAPHORES_CAPACITY; i++) {
    if (semCollection->semaphores[i] == NULL) {
      semdata_t *sem = (semdata_t *)allocMemory(sizeof(semdata_t));
      if (sem == NULL) {
        return 0;
      }
      sem->sem = i;
      sem->name = name;
      sem->isLinked = 1;
      sem->value = initialValue;
      sem->attachedProcesses = 1;
      sem->waitingQueue = createQueue(comparePCB);
      semCollection->semaphores[i] = sem;
      sem->lock = 1;
      semCollection->semaphoresCount++;
      return 1;
    }
  }
  return 0;
}
void semPost(char *name) {
  int aux = lookupSemaphore(name);
  if (aux == -1) {
    return;
  }

  semdata_t *sem = semCollection->semaphores[aux];
  uint8_t *semLock = getSemLock(semCollection, sem);
  acquire(semLock);
  if (sizeQ(sem->waitingQueue) > 0) {
    PCB aux = dequeue(sem->waitingQueue);
    unblockProcess(aux->pid);
    release(semLock);
    return;
  }
  sem->value++;
  release(semLock);
  return;
}
void semWait(char *name) {
  int aux = lookupSemaphore(name);
  if (aux == -1) {
    return;
  }
  semdata_t *sem = semCollection->semaphores[aux];
  uint8_t *semLock = getSemLock(semCollection, sem);
  acquire(semLock);
  if (sem->value > 0) {
    sem->value--;
    release(semLock);
    return;
  }
  queue(sem->waitingQueue, lookUpOnHashMap(getActivePid()));
  release(semLock);
  blockProcess(getActivePid(), SEM);
  return;
}
void semClose(char *name) {
  int aux = lookupSemaphore(name);
  if (aux == -1) {
    return;
  }
  semdata_t *sem = semCollection->semaphores[aux];
  uint8_t *semLock = getSemLock(semCollection, sem);
  acquire(semLock);
  sem->attachedProcesses--;
  if (sem->attachedProcesses > 0) {
    release(semLock);
    return;
  }
  semCollection->semaphores[aux] = NULL;
  semCollection->semaphoresCount--;
  release(semLock);
  freeQueue(sem->waitingQueue);
  freeMemory(sem);

  return;
}