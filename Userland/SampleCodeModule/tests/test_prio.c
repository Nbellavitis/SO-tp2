#include "../include/lib.h"
#include "../include/test_util.h"
#include "../include/usrSysCall.h"
#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 100      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio() {
    while(1);
//  pid_t pids[TOTAL_PROCESSES];
//  char *argv[] = {0};
//  uint64_t i;
//  putC('c',0xFFFFFFFF);
//  for (i = 0; i < TOTAL_PROCESSES; i++)
//    pids[i] = createProcess((uint64_t)endless_loop_print,0,1,0, NULL);
//  bussy_wait(WAIT);
//  print(0xFFFFFFFF,"\nCHANGING PRIORITIES...\n");
//
//  for (i = 0; i < TOTAL_PROCESSES; i++)
//    changePrio(pids[i], prio[i]);
//
//  bussy_wait(WAIT);
//  print(0x80008000,"\nBLOCKING...\n");
//
//  for (i = 0; i < TOTAL_PROCESSES; i++)
//    blockProcess(pids[i]);
//
//  print(0x80008000,"CHANGING PRIORITIES WHILE BLOCKED...\n");
//
//  for (i = 0; i < TOTAL_PROCESSES; i++)
//    changePrio(pids[i], MEDIUM);
//
//  print(0x80008000,"UNBLOCKING...\n");
//
//  for (i = 0; i < TOTAL_PROCESSES; i++)
//    unblockProcess(pids[i]);
//
//  bussy_wait(WAIT);
//  print(0x80008000,"\nKILLING...\n");
//
//  for (i = 0; i < TOTAL_PROCESSES; i++)
//    killProcess(pids[i]);
}
