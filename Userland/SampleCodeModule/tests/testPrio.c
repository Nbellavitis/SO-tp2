// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "../include/lib.h"
#include "../include/testUtil.h"
#include "../include/usrSysCall.h"
#define MINOR_WAIT                                                             \
  1000000 // TODO: Change this value to prevent a process from flooding the
          // screen
#define WAIT                                                                   \
  100000000 // TODO: Change this value to make the wait long enough to see
            // theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 1  // TODO: Change as required
#define MEDIUM 2  // TODO: Change as required
#define HIGHEST 3 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};
static void endlessLoopPrint();
void testPrio() {

  pid_t pids[TOTAL_PROCESSES];
  char *argv[] = {"endless_loop_print"};
  uint64_t i;
  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = createProcess((uint64_t)endlessLoopPrint, 0, 1, argv,
                            (char *[]){"tty", "tty"});
  bussyWait(WAIT);
  print(0xFFFFFFFF, "\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    changePrio(pids[i], prio[i]);

  bussyWait(WAIT);
  print(0x80008000, "\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    blockProcess(pids[i]);

  print(0x80008000, "CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    changePrio(pids[i], MEDIUM);

  print(0x80008000, "UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblockProcess(pids[i]);

  bussyWait(WAIT);
  print(0x80008000, "\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    killProcess(pids[i]);

  putC('\n', 0x80008000);
}
static void endlessLoopPrint() {
  int64_t pid = getMyPid();

  while (1) {
    print(0x80008000, "%d", pid);
    bussyWait(MINOR_WAIT);
  }
}