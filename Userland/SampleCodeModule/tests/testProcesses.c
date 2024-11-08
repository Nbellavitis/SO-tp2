// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "../include/lib.h"
#include "../include/testUtil.h"
#include "../include/usrSysCall.h"
#include <stdio.h>
enum State { RUNNING, BLOCKED, KILLED };

typedef struct pRq {
  pid_t pid;
  enum State state;
} pRq;

int64_t testProcesses(uint64_t argc, char *argv[]) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t maxProcesses;
  char *argvAux[] = {"hijo de test_process", 0};

  if (argc != 1)
    return -1;

  if ((maxProcesses = satoi(argv[0])) <= 0)
    return -1;
  pRq pRqs[maxProcesses];
  while (1) {
    // Create max_processes processes

    for (rq = 0; rq < maxProcesses; rq++) {

      pRqs[rq].pid = createProcess((uint64_t)endlessLoop, 0, 1, argvAux,
                                   (char *[]){"tty", "tty"});

      if (pRqs[rq].pid == -1) {
        print(0xFFFFFFFF, "test_processes: ERROR creating process\n");
        return -1;
      } else {
        pRqs[rq].state = RUNNING;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been
    // killed
    while (alive > 0) {
      for (rq = 0; rq < maxProcesses; rq++) {
        action = GetUniform(100) % 2;
        switch (action) {
        case 0:
          if (pRqs[rq].state == RUNNING || pRqs[rq].state == BLOCKED) {
            if (killProcess(pRqs[rq].pid) == -1) {
              print(0x80008000, "test_processes: ERROR killing process\n");
              return -1;
            }
            pRqs[rq].state = KILLED;
            alive--;
            bussyWait(5000);
          }
          break;

        case 1:
          if (pRqs[rq].state == RUNNING) {
            if (blockProcess(pRqs[rq].pid) == -1) {
              print(0x80008000, "testProcesses: ERROR blocking process\n");
              return -1;
            }
            pRqs[rq].state = BLOCKED;
          }
          break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < maxProcesses; rq++)
        if (pRqs[rq].state == BLOCKED && GetUniform(100) % 2) {
          if (unblockProcess(pRqs[rq].pid) == -1) {
            print(0x80008000, "test_processes: ERROR unblocking process\n");
            return -1;
          }
          pRqs[rq].state = RUNNING;
        }
    }
  }
}
