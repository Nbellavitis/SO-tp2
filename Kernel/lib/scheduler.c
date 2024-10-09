#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/libasm.h"
#include "../collections/queue.h"
#include "../Drivers/include/videoDriver.h"
#define KERNEL_PID -1
#define ACTIVE 1
#define INACTIVE 0
static queueADT processQueue = NULL;
static pid_t activePid = KERNEL_PID;
static size_t status = INACTIVE;
static PCB activeProcess = NULL;
static void idle();

int64_t comparePCB(void * pcb1, void * pcb2) {
    if(pcb1 == NULL || pcb2 == NULL) {
        return (pcb1 != NULL) - (pcb2 != NULL);
    }
    PCB a = (PCB )pcb1;
    PCB b = (PCB )pcb2;
    return (a->pid > b->pid) - (a->pid < b->pid);
}

void startScheduler() {
    status = ACTIVE;
    processQueue = createQueue(comparePCB);
    newProcess((uint64_t)idle, 1, 1, 0, NULL);
}

uint64_t contextSwitch(uint64_t rsp){
    if ( status == INACTIVE)
        return rsp;
    char  aux[10];
    intToStr(getActivePid(),aux);
    drawWord(0xFFFFFFFF,aux);
    if ( activePid == KERNEL_PID){
        activeProcess = dequeue(processQueue);
        activePid = 0;
        if (activeProcess->rip == (uint64_t) idle ){
            queue(processQueue,activeProcess);
            activeProcess = dequeue(processQueue);
        }
        activePid = activeProcess->pid;
        activeProcess->status = RUNNING;
        return activeProcess->rsp;
    }
    activeProcess->rsp = rsp;
    if (activeProcess->status != KILLED){
        if(activeProcess->status != BLOCKED){
            activeProcess->status = READY;
        }
        queue(processQueue,activeProcess);
    }else{
        //freeProcess(activeProcess);
    }
    while (1){
    activeProcess = dequeue(processQueue);
    if (activeProcess->status == READY) {
        // if(activeProcess->rip == (uint64_t)idle){
        //     queue(processQueue,activeProcess);
        //     activeProcess = dequeue(processQueue);
        // }
        activePid = activeProcess->pid;
        activeProcess->status = RUNNING;
        return activeProcess->rsp;
    }else if (activeProcess->status == BLOCKED){
        queue(processQueue,activeProcess);
    } else if (activeProcess->status == KILLED) {
        char aux[10];
        intToStr(activeProcess->pid,aux);
        drawWord(0xFFFFFFFF,aux);
        //freeProcess(activeProcess);
    }
}}

static void idle() {
    while (1) {
        _hlt();
    }
}
pid_t getActivePid(){
    return activeProcess->pid;
}
void addToReadyQueue(PCBType * pcb){
    int counter = pcb->priority;
    while(counter > 0) {
        queue(processQueue, pcb);
        counter--;
    }
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

// PCB findPcb(queueADT queue,pid_t pid){
//     if(isEmpty(queue))
//         return NULL;
//     toBegin(queue);
//     while(hasNext(queue)){
//         PCB aux = next(queue);
//         if(aux->pid == pid){
//             return aux;
//         }
//     }
//     return NULL;
// }


// size_t searchForReadyProcess(){
//     toBegin(processQueue);
//     while(hasNext(processQueue)){
//         if(((PCBType *) next(processQueue))->status == READY){
//             return 1;
//         }
//     }
//     return 0;
// }
void printQueue(){
    toBegin(processQueue);
    char aux[10];
    while(hasNext(processQueue)){
        intToStr(((PCB) next(processQueue))->status,aux);
        drawWord(0xFFFFFFFF,aux);
        drawChar(0xFFFFFFFF,'\n');
    }
}