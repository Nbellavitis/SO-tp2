#include "../include/process.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
typedef struct PCBType{
    pid_t pid, ppid;
    uint64_t rsp, stackBase, rip;
    int ground;
    int priority;
    int status;
    char * name;

    int fd[3];

    // Waiting Processes
    //queueADT childProcessesWaiting;
}PCBType;

static int  nextProcessId = 0;
int64_t comparePid(void* pid1, void* pid2);
pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]){
    PCB pcb = (PCBType *) allocMemory(sizeof(PCBType));
    if (pcb == NULL)
        return -1;
    pcb->stackBase = (uint64_t) allocMemory(STACK_SIZE) + STACK_SIZE;
    if (pcb->stackBase - STACK_SIZE == 0){
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
    pcb->pid = nextProcessId;

    //pcb->childProcessesWaiting= newQueue(comparePid);

    pcb->ppid = getActivePid(); //te dice el scheduler quien esta corriendo

    pcb->rsp = createProcess(pcb->stackBase, pcb->rip, argc, argv);

    pcb->fd[STDIN] = STDIN;
    pcb->fd[STDOUT] = STDOUT;
    pcb->fd[STDERR] = STDERR;


    addToReadyQueue(pcb);

    return nextProcessId++;
}

int64_t comparePid(void * pid1, void * pid2) {
    if(pid1 == NULL || pid2 == NULL) {
        return -1;
    }
    pid_t a = *(pid_t *)pid1;
    pid_t b = *(pid_t *)pid2;
    return (a > b) - (a < b);
}
void freeProcess(PCB process){
    freeMemory((void *)process->stackBase);
    freeMemory(process);
}