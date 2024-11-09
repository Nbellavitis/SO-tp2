
#ifndef SEMS_H
#define SEMS_H
#include "../collections/queue.h"
#include "../mm/mm.h"
#include <stdlib.h>
#include "lib.h"
#include "process.h"
#include "interrupts.h"
#include "scheduler.h"
#define SEMAPHORES_CAPACITY 32
int semInit();
void semWait(char * sem);
void semPost(char * sem);
void semClose(char * sem);
int semOpen(char * name, int value);
#endif //SEMS_H
