#include "../include/process.h"
#include "../include/lib.h"
#include "../include/scheduler.h"
#include "../collections/hashMap.h"
#include "../Drivers/include/videoDriver.h"
#define MAX_PROCESSES 1000
static HashMapADT PCBMap;
static int  nextProcessId = 0;
static int aliveProcesses = 0;
int64_t comparePid(void* pid1, void* pid2);
pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]){
    PCB pcb = (PCBType *) allocMemory(sizeof(PCBType));
    if (pcb == NULL){
        return -1;
        }
    pcb->stackBase = (uint64_t) allocMemory(STACK_SIZE);
    if (pcb->stackBase == 0){
        drawWord(0xFFFFFF,"PORQUE ME PASA ESTOOO");
        freeMemory(pcb);
        return -1;
    }
    pcb->stackBase += STACK_SIZE;
    pcb->rip = rip;
    pcb->ground = ground;
    pcb->status = READY;
    pcb->priority = priority;
    pcb->pid = nextProcessId;
    pcb->argv= argv;
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
    aliveProcesses++;
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
void freeProcess(PCB pcb){
    delete(PCBMap,&(pcb->pid));
    freeMemory((void *)(pcb->stackBase - STACK_SIZE));
    freeMemory(pcb->argv);
    freeMemory(pcb);
    aliveProcesses--;
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
    if(pid == getActivePid()){
        yield();
    }
   return 0;
}

int8_t blockProcess(pid_t pid) {
    PCB aux = lookup(PCBMap,&pid);
    if(aux == NULL){
        return -1;
    }
    aux->status = BLOCKED;
    if(pid == getActivePid()){
        yield();
    }
    return 0;
}

int8_t changePrio(pid_t pid,int priority){
    PCB aux = lookup(PCBMap,&pid);
    if(aux == NULL){
        return -1;
    }
    if(aux->priority > priority){
        while(aux->priority - priority != 0){
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

PCB lookUpOnHashMap(pid_t * pid){
    return lookup(PCBMap,pid);
}
