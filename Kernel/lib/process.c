#include "../include/process.h"

typedef PCBType * PCB

pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]){

    PCB pcb = (PCBType *) allocMemory(sizeof(PCBType));
    if (pcb == NULL)
        return -1;
    pcb->stack_base = (uint64_t) allocMemory(STACK_SIZE) + STACK_SIZE;
    if (pcb->stack_base)
}