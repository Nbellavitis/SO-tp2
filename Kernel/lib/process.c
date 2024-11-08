#include "../include/process.h"
#include "../Drivers/include/videoDriver.h"
#include "../collections/hashMap.h"
#include "../collections/queue.h"
#include "../include/lib.h"
#include "../include/pipe.h"
#include "../include/scheduler.h"
#define MAX_PROCESSES 7000
static HashMapADT PCBMap;

static int nextProcessId = 0;
static int aliveProcesses = 0;
#define SHELL_PID 1

pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char *argv[],
                 char *descriptors[2]) {
  if (aliveProcesses < MAX_PROCESSES) {
    PCB pcb = (PCBType *)allocMemory(sizeof(PCBType));
    if (pcb == NULL) {
      return -1;
    }
    pcb->stackBase = (uint64_t)allocMemory(STACK_SIZE);
    if (pcb->stackBase == 0) {
      freeMemory(pcb);
      return -1;
    }
    pcb->stackBase += STACK_SIZE;
    pcb->rip = rip;
    pcb->ground = ground;
    pcb->status = READY;
    pcb->priority = priority;
    pcb->pid = nextProcessId;
    pcb->argv = argv;
    if (argv != NULL) {
      pcb->name = argv[0];
    }
    pcb->waitingFor = 0;
    pcb->waitingProcesses = createQueue(comparePCB);
    if (pcb->pid == 0 || pcb->pid == 1) {
      pcb->ppid = -1;
    } else {
      pcb->ppid = getActivePid();
    }

    pcb->rsp = createProcess(pcb->stackBase, pcb->rip, argc, argv);
    pcb->fd[STDIN] = descriptors[0];
    pcb->fd[STDOUT] = descriptors[1];
    pcb->fd[STDERR] = "tty";

    insert(PCBMap, (pcb->pid), pcb);
    addToReadyQueue(pcb);

    aliveProcesses++;
    return nextProcessId++;
  }
  return -1;
}

int64_t comparePid(pid_t pid1, pid_t pid2) {
  pid_t a = pid1;
  pid_t b = pid2;
  return (a > b) - (a < b);
}
void freeProcess(PCB pcb) {
  delete (PCBMap, pcb->pid);
  freeMemory((void *)(pcb->stackBase - STACK_SIZE));
  freeQueue(pcb->waitingProcesses);
  freeMemory(pcb);
  aliveProcesses--;
}
void initMap() { PCBMap = create_hash_map(comparePid); }
int8_t unblockProcess(pid_t pid) {
  PCB aux = lookup(PCBMap, pid);
  if (aux == NULL) {
    return -1;
  }
  if (aux->status != BLOCKED) {
    return -1;
  }
  aux->status = READY;
  return 0;
}

int8_t killProcess(pid_t pid) {
  PCB aux = lookup(PCBMap, pid);
  if (aux == NULL || pid == 0) {
    return -1;
  }
  aux->status = KILLED;
  toBegin(aux->waitingProcesses);
  while (hasNext(aux->waitingProcesses)) {
    PCB toUnblock = next(aux->waitingProcesses);
    if (toUnblock->status == BLOCKED) {
      unblockProcess(toUnblock->pid);
    }
  }
  if (aux == getCurrentForegroundProcess()) {
    setNullForegroundProcess();
  }
  if (pid == getActivePid()) {
    yield();
  }
  return 0;
}

int8_t blockProcess(pid_t pid, int reason) {

  PCB aux = lookup(PCBMap, pid);

  if (aux == NULL) {
    return -1;
  }
  if (aux->status == EXITED || aux->status == KILLED) {
    return -1;
  }

  aux->status = BLOCKED;
  aux->waitingFor = reason;

  if (pid == getActivePid()) {
    yield();
  }
  return 0;
}

int8_t changePrio(pid_t pid, int priority) {
  PCB aux = lookup(PCBMap, pid);
  if (aux == NULL) {
    return -1;
  }
  aux->priority = priority;

  return 0;
}

PCB lookUpOnHashMap(pid_t pid) { return lookup(PCBMap, pid); }

processInfoPtr getProcessInfo(pid_t pid) {
  PCB aux = lookup(PCBMap, pid);
  if (aux == NULL) {
    return NULL;
  }
  processInfoPtr toRet = (processInfoPtr)allocMemory(sizeof(PCBType));
  if (toRet == NULL) {
    drawWord(0x00FF0000, "Error allocating memory\n");
    return NULL;
  }
  toRet->pid = aux->pid;
  toRet->ppid = aux->ppid;
  toRet->rsp = aux->rsp;
  toRet->stackBase = aux->stackBase;
  toRet->rip = aux->rip;
  toRet->ground = aux->ground;
  toRet->priority = aux->priority;
  toRet->status = aux->status;
  toRet->name = aux->name;
  return toRet;
}

processInfoPtr *getAllProcessInfo() {
  processInfoPtr *toRet =
      (processInfoPtr *)allocMemory(sizeof(PCB) * (aliveProcesses));
  if (toRet == NULL) {
    drawWord(0x00FF0000, "Error allocating memory\n");
    return NULL;
  }
  uint64_t j = 0;
  for (uint64_t i = 0; i < nextProcessId; i++) {
    processInfoPtr current = getProcessInfo(i);
    if (current != NULL) {
      toRet[j++] = current;
    }
  }
  toRet[j] = NULL;
  return toRet;
}
uint64_t waitpid(pid_t pid) {
  PCB aux = lookup(PCBMap, pid);
  uint64_t ret;
  if (aux == NULL) {
    return -1;
  }
  PCB activeProcess = getActiveProcess();
  if (activeProcess == NULL) {
    return -1;
  }
  if (aux->status == EXITED) {
    ret = aux->ret;
    killProcess(aux->pid);
    return ret;
  }
  queue(aux->waitingProcesses, activeProcess);
  if (aux->ground == 1 || aux->ppid != SHELL_PID) {
    activeProcess->waitingFor = CHILD_PROCESS;
    blockProcess(activeProcess->pid, CHILD_PROCESS);
    activeProcess->waitingFor = 0;
    ret = aux->ret;
    killProcess(aux->pid);
    return ret;
  }
  return -1;
}

void exitProcess(uint64_t retStatus) {
  PCB activeProcess = getActiveProcess();
  activeProcess->ret = retStatus;
  if (strcmp(activeProcess->fd[STDIN], "tty") != 0) {
    pipeClose(activeProcess->fd[STDIN]);
  }
  if (strcmp(activeProcess->fd[STDOUT], "tty") != 0) {
    pipeClose(activeProcess->fd[STDOUT]);
  }
  if (activeProcess->ground == 0 && activeProcess->ppid == SHELL_PID) {
    killProcess(activeProcess->pid);
    return;
  }
  toBegin(activeProcess->waitingProcesses);
  while (hasNext(activeProcess->waitingProcesses)) {
    PCB toUnblock = next(activeProcess->waitingProcesses);
    if (toUnblock->status == BLOCKED) {
      unblockProcess(toUnblock->pid);
    }
  }
  if (activeProcess == getCurrentForegroundProcess()) {
    setNullForegroundProcess();
  }
  activeProcess->status = EXITED;
  yield();
}
char **getMyFds() {
  PCB activeProcess = getActiveProcess();
  if (activeProcess == NULL) {
    return NULL;
  }
  return activeProcess->fd;
}