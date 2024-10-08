#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "../collections/queue.h"
#include <stdint.h>
#include "processAsm.h"
#include "lib.h"
#define READY 0
#define BLOCKED 1
#define KILLED 2
#define SLEEPING 3
#define RUNNING 4
#define STACK_SIZE 4096
#define KERNEL_PID -1
#define IDLE_PID 1

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

typedef struct PCBType * PCB;

int64_t comparePid(void * pid1, void * pid2);
pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]);
void freeProcess(PCB process);
int8_t unblockProcess(pid_t pid);
int8_t killProcess(pid_t pid);
int8_t blockProcess(pid_t pid);
int8_t changePrio(pid_t pid,int priority);
#endif