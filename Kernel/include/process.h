#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include "../collections/queue.h"
#include <stdint.h>


#define READY 0
#define BLOCKED 1
#define KILLED 2
#define SLEEPING 3
#define STACK_SIZE 4096

typedef struct processData{
    pid_t pid, ppid;
    uint64_t rsp, stack_base, rip;
    int ground;
    int priority;
    int status;
    char * name;

    int fd[3];

    // Waiting Processes
    queueADT childProcessesWaiting;
}PCBType;


pid_t newProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]);