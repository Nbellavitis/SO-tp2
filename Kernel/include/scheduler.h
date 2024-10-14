#ifndef SO_TP2_SCHEDULER_H
#define SO_TP2_SCHEDULER_H
#include <stdlib.h>
#include "lib.h"
#include "process.h"
#include "../collections/queue.h"
#include "interrupts.h"
#include "irqDispatcher.h"
pid_t getActivePid();
uint64_t contextSwitch(uint64_t rsp);
void startScheduler();
void addToReadyQueue(PCB pcb);
void removeAll(PCB pcb);
PCB getActiveProcess();
int64_t comparePCB(void * pcb1, void * pcb2);
#endif //SO_TP2_SCHEDULER_H
