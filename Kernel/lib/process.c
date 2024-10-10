#include "../include/process.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
#include "../collections/hashMap.h"
#include "../Drivers/include/videoDriver.h"
static HashMapADT PCBMap;
static int  nextProcessId = 0;
int64_t comparePid(void* pid1, void* pid2);
pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]){
    PCB pcb = (PCBType *) allocMemory(sizeof(PCBType));
    if (pcb == NULL){
        drawWord(0x0000FF00, "Error al crear el proceso");
        return -1;
        }
    pcb->stackBase = (uint64_t) allocMemory(STACK_SIZE) + STACK_SIZE;
    if (pcb->stackBase - STACK_SIZE == 0){
        drawWord(0x0000FF00, "Error al crear el stack");
        freeMemory(pcb);
        return -1;
    }
    pcb->rip = rip;
    pcb->ground = ground;
    pcb->status = READY;
    pcb->priority = priority;
    pcb->pid = nextProcessId;
    //pcb->childProcessesWaiting = newQueue(comparePid);

    //ACA DEBERIAMOS AGREGARLO AL CHILDPROCESSES
    pcb->ppid = getActivePid(); //te dice el scheduler quien esta corriendo
    pcb->rsp = createProcess(pcb->stackBase, pcb->rip, argc, argv);
    pcb->fd[STDIN] = STDIN;
    pcb->fd[STDOUT] = STDOUT;
    pcb->fd[STDERR] = STDERR;

    insert(PCBMap,&(pcb->pid),pcb);
    while(priority > 0){
        addToReadyQueue(pcb);
        priority--;
    }
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
    delete(PCBMap, process->pid);
    freeMemory((void *)process->stackBase);
    freeMemory(process);
}
void initMap(){
    PCBMap = create_hash_map(comparePid);
}
int8_t unblockProcess(pid_t pid){
   PCB aux = lookup(PCBMap,&pid);
    if(aux == NULL){
        return -1;
    }
    aux->status=READY;
    return 0;
}

int8_t killProcess(pid_t pid) {
PCB aux = lookup(PCBMap,&pid);
    if(aux == NULL){
        return -1;
    }
    aux->status=KILLED;
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

int8_t blockProcess(pid_t pid) {
    PCB aux = lookup(PCBMap,&pid);
    if(aux == NULL){
        return -1;
    }
    aux->status = BLOCKED;
    return 0;
}

int8_t changePrio(pid_t pid,int priority){
    PCB aux = lookup(PCBMap,&pid);
    if(aux == NULL){
        return -1;
    }
    if(aux->priority > priority){
        while(aux->priority-priority != 0){
            removeFromReadyQueue(aux);
            aux->priority--;
        }
    }else{
        while(priority-aux->priority != 0){
            addToReadyQueue(aux);
            aux->priority++;
        }
    }
    return 0;
}



