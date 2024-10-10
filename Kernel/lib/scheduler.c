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
static void idle();
static void removeAll(queueADT queue);
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
    //idleProcess = lookUpOnHashMap(newProcess((uint64_t)idle, 1, 1, 0, NULL));
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
    if (activeProcess->status != KILLED && activeProcess->pid != idleProcess->pid){
        if(activeProcess->status != BLOCKED){
            activeProcess->status = READY;
        }
        queue(processQueue,activeProcess);
    }else{
        removeAll(processQueue);
        freeProcess(activeProcess);
    }
    int size = sizeQ(processQueue);
    for(int i=0;i < size; i++ ){
    activeProcess = dequeue(processQueue);
    if (activeProcess->status == READY) {
        activePid = activeProcess->pid;
        activeProcess->status = RUNNING;
        return activeProcess->rsp;
    }else if (activeProcess->status == BLOCKED){
        queue(processQueue,activeProcess);
    } else if (activeProcess->status == KILLED) {
        removeAll(processQueue);
        freeProcess(activeProcess);
    }
}
    // activeProcess=idleProcess;
    // activePid=idleProcess->pid;
    // return activeProcess->rsp;
}

static void idle() {
    while (1) {
        _hlt();
    }
}
static void removeAll(queueADT queue){
    int counter=1;
    while(activeProcess->priority - counter != 0){
        remove(processQueue,activeProcess);
        counter++;
    }
}
pid_t getActivePid(){
    return activeProcess->pid;
}
void addToReadyQueue(PCBType * pcb){
    queue(processQueue, pcb);
}

PCBType * findProcessByPid(pid_t pid){
    if(processQueue == NULL || isEmpty(processQueue)){
        return NULL;
    }
    toBegin(processQueue);
    while(hasNext(processQueue)){
        PCBType * currentProcess = (PCBType *) next(processQueue);
        if(currentProcess->pid == pid){
             return currentProcess;
        }
    }
    return NULL;
}


void yield(){
    _irq00Handler();
}


void printQueue(){
    toBegin(processQueue);
    char aux[10];
    while(hasNext(processQueue)){
        intToStr(((PCB) next(processQueue))->status,aux);
        drawWord(0xFFFFFFFF,aux);
        drawChar(0xFFFFFFFF,'\n');
    }
}
int8_t removeFromReadyQueue(PCB pcb){
    return remove(processQueue,pcb);
}