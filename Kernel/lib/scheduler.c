#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/libasm.h"
#include "../collections/queue.h"
#include "../Drivers/include/videoDriver.h"
#define KERNEL_PID -1
#define ACTIVE 1
#define INACTIVE 0
static queueADT processQueue = NULL;
static PCB idleProcess;
static pid_t activePid = KERNEL_PID;
static size_t status = INACTIVE;
static PCB activeProcess = NULL;
static int timesActiveExecuted =0;
static void idle();
int64_t comparePCB(void * pcb1, void * pcb2) {
    if (pcb1 == NULL || pcb2 == NULL) {
        return (pcb1 != NULL) - (pcb2 != NULL);
    }
    PCB a = (PCB)pcb1;
    PCB b = (PCB)pcb2;
    return (a->pid > b->pid) - (a->pid < b->pid);
}

void startScheduler() {
    status = ACTIVE;
    processQueue = createQueue(comparePCB);
    idleProcess = lookUpOnHashMap((pid_t) newProcess((uint64_t)idle, 1, 1, 0, NULL));
}

static PCB findNextReadyProcess(queueADT processQueue) {
    int size = sizeQ(processQueue);
    for(int i=0;i < size; i++ ){
        activeProcess = dequeue(processQueue);
        if(activeProcess != NULL) {
            if (activeProcess->status == READY) {
                activePid = activeProcess->pid;
                activeProcess->status = RUNNING;
                return activeProcess;
            } else if (activeProcess->status == BLOCKED) {
                queue(processQueue, activeProcess);
            } else if (activeProcess->status == KILLED) {
                freeProcess(activeProcess);
            } else if (activeProcess->status == EXITED) {
                PCB parent = lookUpOnHashMap(activeProcess->ppid);
                if (parent == NULL || parent->status == EXITED) {
                    killProcess(activeProcess->pid);
                }
                queue(processQueue, activeProcess);
            }
        }
    }
    return NULL;
}

uint64_t contextSwitch(uint64_t rsp){
    if ( status == INACTIVE)
        return rsp;
    if ( activePid == KERNEL_PID){
        activeProcess = dequeue(processQueue);
        activePid = 0;
        if (activeProcess->rip == (uint64_t) idle ){
            activeProcess = dequeue(processQueue);
        }
        activePid = activeProcess->pid;
        activeProcess->status = RUNNING;
        return activeProcess->rsp;
    }
    activeProcess->rsp = rsp;
    if (activeProcess->status != KILLED ){
        if(activeProcess->status != EXITED) {
            if (activeProcess->status != BLOCKED) {
                if (activeProcess->priority - 1 > timesActiveExecuted) {
                    timesActiveExecuted++;
                    return activeProcess->rsp;
                }
                activeProcess->status = READY;
            }
            if (activeProcess->pid != idleProcess->pid) {
                timesActiveExecuted = 0;
                queue(processQueue, activeProcess);
            }

        } else{
            queue(processQueue, activeProcess);
        }
    }else{
        freeProcess(activeProcess);
    }
    activeProcess = findNextReadyProcess(processQueue);
    if(activeProcess == NULL){
        activeProcess=idleProcess;
        activePid=idleProcess->pid;
    }
    return activeProcess->rsp;
}

static void idle() {
    while (1) {
        _hlt();
    }
}

pid_t getActivePid(){
    return activeProcess->pid;
}

void addToReadyQueue(PCBType * pcb){
    queue(processQueue, pcb);
}

PCBType * findProcessByPid(pid_t pid) {
    if (processQueue == NULL || isEmpty(processQueue)) {
        return NULL;
    }
    toBegin(processQueue);
    while (hasNext(processQueue)) {
        PCBType *currentProcess = (PCBType *) next(processQueue);
        if (currentProcess->pid == pid) {
            return currentProcess;
        }
    }
    return NULL;
}

PCB getActiveProcess(){
    return activeProcess;
}

int8_t removeFromReadyQueue(PCB pcb){
    return remove(processQueue,pcb);
}