// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "../include/scheduler.h"
#include "../Drivers/include/videoDriver.h"
#include "../collections/queue.h"
#include "../include/libasm.h"
#include "../include/process.h"
#define KERNEL_PID -1
#define ACTIVE 1
#define INACTIVE 0
typedef struct {
  queueADT processQueue;
  PCB idleProcess;
  pid_t activePid;
  PCB activeProcess;
  int timesActiveExecuted;
  PCB currentForegroundProcess;
} SchedulerData;
static SchedulerData data;
static int8_t status = INACTIVE;
static void idle();
int64_t comparePCB(void *pcb1, void *pcb2) {
  if (pcb1 == NULL || pcb2 == NULL) {
    return (pcb1 != NULL) - (pcb2 != NULL);
  }
  PCB a = (PCB)pcb1;
  PCB b = (PCB)pcb2;
  return (a->pid > b->pid) - (a->pid < b->pid);
}

void startScheduler() {
  data.timesActiveExecuted = 0;
  data.processQueue = NULL;
  data.activePid = KERNEL_PID;
  data.activeProcess = NULL;
  data.currentForegroundProcess = NULL;
  status = ACTIVE;
  data.processQueue = createQueue(comparePCB);
  if (data.processQueue == NULL) {
    drawWord(0xFF0000, "Error creating process queue");
    return;
  }
  char *argv[] = {"idle", "null"};
  data.idleProcess = lookUpOnHashMap((pid_t)newProcess(
      (uint64_t)idle, 0, 1, 1, argv, (char *[]){"tty", "null"}));
}

static PCB findNextReadyProcess() {
  int size = sizeQ(data.processQueue);
  for (int i = 0; i < size; i++) {
    data.activeProcess = dequeue(data.processQueue);
    if (data.activeProcess != NULL) {
      if (data.activeProcess->status == READY) {
        data.activePid = data.activeProcess->pid;
        data.activeProcess->status = RUNNING;
        if (data.activeProcess->ground == FOREGROUND) {
          data.currentForegroundProcess = data.activeProcess;
        }
        return data.activeProcess;
      } else if (data.activeProcess->status == BLOCKED) {
        queue(data.processQueue, data.activeProcess);
      } else if (data.activeProcess->status == KILLED) {
        freeProcess(data.activeProcess);
      } else if (data.activeProcess->status == EXITED) {
        PCB parent = lookUpOnHashMap(data.activeProcess->ppid);
        if (parent == NULL || parent->status == EXITED) {
          killProcess(data.activeProcess->pid);
        }
        queue(data.processQueue, data.activeProcess);
      }
    }
  }
  return NULL;
}

uint64_t contextSwitch(uint64_t rsp) {

  if (status == INACTIVE)
    return rsp;

  if (data.activePid == KERNEL_PID) {
    data.activeProcess = dequeue(data.processQueue);
    data.activePid = 0;
    if (data.activeProcess->rip == (uint64_t)idle) {
      data.activeProcess = dequeue(data.processQueue);
    }
    data.activePid = data.activeProcess->pid;
    data.activeProcess->status = RUNNING;
    if (data.activeProcess->ground == FOREGROUND) {
      data.currentForegroundProcess = data.activeProcess;
    }
    return data.activeProcess->rsp;
  }
  data.activeProcess->rsp = rsp;
  if (data.activeProcess->status != KILLED) {
    if (data.activeProcess->status != EXITED) {
      if (data.activeProcess->status != BLOCKED) {
        if (data.activeProcess->priority - 1 > data.timesActiveExecuted) {
          data.timesActiveExecuted++;
          if (data.activeProcess->ground == FOREGROUND) {
            data.currentForegroundProcess = data.activeProcess;
          }
          return data.activeProcess->rsp;
        }
        data.activeProcess->status = READY;
      }
      if (data.activeProcess->pid != data.idleProcess->pid) {
        data.timesActiveExecuted = 0;
        queue(data.processQueue, data.activeProcess);
      }

    } else {
      queue(data.processQueue, data.activeProcess);
    }
  } else {
    freeProcess(data.activeProcess);
  }
  data.activeProcess = findNextReadyProcess();
  if (data.activeProcess == NULL) {
    data.activeProcess = data.idleProcess;
    data.activePid = data.idleProcess->pid;
  }
  if (data.activeProcess->ground == FOREGROUND) {
    data.currentForegroundProcess = data.activeProcess;
  }
  return data.activeProcess->rsp;
}

static void idle() {
  while (1) {
    _hlt();
  }
}

pid_t getActivePid() { return data.activeProcess->pid; }

void addToReadyQueue(PCBType *pcb) { queue(data.processQueue, pcb); }

PCBType *findProcessByPid(pid_t pid) {
  if (data.processQueue == NULL || isEmpty(data.processQueue)) {
    return NULL;
  }
  toBegin(data.processQueue);
  while (hasNext(data.processQueue)) {
    PCBType *currentProcess = (PCBType *)next(data.processQueue);
    if (currentProcess->pid == pid) {
      return currentProcess;
    }
  }
  return NULL;
}

PCB getActiveProcess() { return data.activeProcess; }

int8_t removeFromReadyQueue(PCB pcb) { return remove(data.processQueue, pcb); }
PCB getCurrentForegroundProcess() { return data.currentForegroundProcess; }
void setNullForegroundProcess() { data.currentForegroundProcess = NULL; }