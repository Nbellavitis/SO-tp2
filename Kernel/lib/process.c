#include "../include/process.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
#include "../collections/hashMap.h"
#include "../Drivers/include/videoDriver.h"
#include "../collections/queue.h"
#define MAX_PROCESSES 1000
static HashMapADT PCBMap;
static int  nextProcessId = 0;
static int aliveProcesses = 0;
int64_t comparePid(pid_t pid1, pid_t pid2);

pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]){
    PCB pcb = (PCBType *) allocMemory(sizeof(PCBType));
    if (pcb == NULL){
        return -1;
        }
    pcb->stackBase = (uint64_t) allocMemory(STACK_SIZE);
    if (pcb->stackBase == 0){
        freeMemory(pcb);
        return -1;
    }
    pcb->stackBase += STACK_SIZE ;
    pcb->rip = rip;
    pcb->ground = ground;
    pcb->status = READY;
    pcb->priority = priority;
    pcb->pid = nextProcessId;
    pcb->argv= argv;
    pcb->waitingProcesses = createQueue(comparePCB);

    pcb->ppid = getActivePid(); 
    pcb->rsp = createProcess(pcb->stackBase, pcb->rip, argc, argv);
    pcb->fd[STDIN] = STDIN;
    pcb->fd[STDOUT] = STDOUT;
    pcb->fd[STDERR] = STDERR;

    insert(PCBMap,(pcb->pid),pcb);
    addToReadyQueue(pcb);

    aliveProcesses++;
    return nextProcessId++;
}

int64_t comparePid(pid_t pid1, pid_t pid2) {
    pid_t a = pid1;
    pid_t b = pid2;
    return (a > b) - (a < b);
}
void freeProcess(PCB pcb){
    delete(PCBMap,pcb->pid);
    freeMemory((void *)(pcb->stackBase - STACK_SIZE));
    freeMemory(pcb->argv);
    freeQueue(pcb->waitingProcesses);
    freeMemory(pcb);
    aliveProcesses--;
}
void initMap(){
    PCBMap = create_hash_map(comparePid);
}
int8_t unblockProcess(pid_t pid){
   PCB aux = lookup(PCBMap,pid);
    if(aux == NULL){
        return -1;
    }
    if(aux->status != BLOCKED){
        return -1;
    }
    aux->status=READY;
    return 0;
}

int8_t killProcess(pid_t pid) {
PCB aux = lookup(PCBMap,pid);
    if(aux == NULL){
        return -1;
    }
    aux->status=KILLED;
    if(pid == getActivePid()){
        nice();
    }
   return 0;
}

int8_t blockProcess(pid_t pid) {
    PCB aux = lookup(PCBMap,pid);
    if(aux == NULL){
        return -1;
    }
    aux->status = BLOCKED;
    if(pid == getActivePid()){
        nice();
    }
    return 0;
}

int8_t changePrio(pid_t pid,int priority){
    PCB aux = lookup(PCBMap,pid);
    if(aux == NULL){
        return -1;
    }
    aux->priority = priority;

    return 0;
}

PCB lookUpOnHashMap(pid_t pid){
    return lookup(PCBMap,pid);
}

processInfoPtr getProcessInfo(pid_t pid){
    PCB aux = lookup(PCBMap,pid);
    if(aux == NULL){
        return NULL;
    }
    processInfoPtr toRet = (processInfoPtr) allocMemory(sizeof(PCBType));
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

processInfoPtr * getAllProcessInfo(){
    processInfoPtr * toRet = (processInfoPtr *) allocMemory(sizeof(PCB) * (aliveProcesses + 1));
    uint64_t j=0;
    for(uint64_t i = 0; i < nextProcessId; i++){
        processInfoPtr current = getProcessInfo(i);
        if(current != NULL){
            toRet[j++] = current;
        }
    }
    toRet[j] = NULL;
    return toRet;
}
uint64_t waitpid(pid_t pid){
    PCB aux = lookup(PCBMap,pid);
    uint64_t ret;
    if(aux == NULL){
        drawWord(0xFFFFFF,"El proceso no existe");
        printNumber(pid,0xFFFFFF);
        return -1;
    }
    PCB activeProcess = getActiveProcess();
    if(activeProcess == NULL){
        drawWord(0xFFFFFF,"No hay proceso activo");
        return -1;
    }
    if(aux->status == EXITED){
        drawWord(0xFFFFFF,"El proceso ya termino");
        ret = aux->ret;
        killProcess(aux->pid);
        return ret;
    }
    queue(aux->waitingProcesses,activeProcess);
    blockProcess(activeProcess->pid);
    ret = aux->ret;
    killProcess(aux->pid);
    return ret;
}


void exitProcess(uint64_t retStatus){
    PCB activeProcess = getActiveProcess();
    activeProcess->ret = retStatus;
    toBegin(activeProcess->waitingProcesses);
    while(hasNext(activeProcess->waitingProcesses)){
        PCB toUnblock = next(activeProcess->waitingProcesses);
        unblockProcess(toUnblock->pid);
    }
    activeProcess->status = EXITED;
    nice();
}