#include "../include/process.h"

typedef PCBType * PCB
static nextProccesPid=0;
int64_t comparePid(void* pid1, void* pid2);
pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]){

    PCB pcb = (PCBType *) allocMemory(sizeof(PCBType));
    if (pcb == NULL)
        return -1;
    pcb->stackBase = (uint64_t) allocMemory(STACK_SIZE) + STACK_SIZE;
    if (pcb->stack_base - STACK_SIZE == 0){
        freeMemory(pcb);
        return -1;
    }
    pcb->rip = rip;
    pcb->ground = ground; //backGround foreGround
      if(getActivePid() == KERNEL_PID)
        pcb->status = READY;
    else{
        pcb->status = BLOCKED;
    }
    pcb->priority = priority;
    pcb->pid = nextProccesId;

    pcb->childProcessesWaiting= newQueue(comparePid);

    pcb->ppid = getActivePid(); //te dice el scheduler quien esta corriendo

    pcb->rsp = createProcess(pcb->stackBase, pcb->rip, pcb->argc, pcb->argv);

    pcb->fd[STDIN] = STDIN;
    pcb->fd[STDOUT] = STDOUT;
    pcb->fd[STDERR] = STDERR;


    addToReadyQueue(&pcb);

    return nextProccesId++;
}

int64_t comparePid(void * pid1, void * pid2) {
    if(pid1 == NULL || pid2 == NULL) {
        return NULL;
    }
    pid_t a = *(pid_t *)pid1;
    pid_t b = *(pid_t *)pid2;
    return (a > b) - (a < b);
}