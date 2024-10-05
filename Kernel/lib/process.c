#include "../include/process.h"
#include "../include/lib.h"
#include "../include/scheduler.h"


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

    //pcb->childProcessesWaiting = newQueue(comparePid);

    //ACA DEBERIAMOS AGREGARLO AL CHILDPROCESSES
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



int8_t blockProcess(PCBType * process) {
    if(process == NULL || process->status == BLOCKED || process->status == KILLED) {
        return -1;
    }

    process->status = BLOCKED;
    //ACA DEBERIAMOS LLAMAR PARA QUE CORRA OTRO PROCESS
    return 0;
}

int8_t killProcess(PCBType * process) {
    if(process == NULL) {
        return -1;
    }
    process->status = KILLED;

    //Tengo que dejarle los hijos al init que los adopte
//    if(process->childProcessesWaiting != NULL && isEmpty(process->childProcessesWaiting)){
//        toBegin(process->childProcessesWaiting);
//        while(hasNext(process->childProcessesWaiting)){
//            PCBType * currentProcess = (PCBType * ) next(process->childProcessesWaiting);
//            currentProcess->ppid = IDLE_PID;
//        }
//    }

    return 0;
}


