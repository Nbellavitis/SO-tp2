// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "../include/lib.h"
#include "../include/testUtil.h"
#include "../include/usrSysCall.h"

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t myProcessInc(uint64_t argc, char *argv[]) {

  uint64_t n;
  int8_t inc;
  int8_t useSem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((useSem = satoi(argv[2])) < 0)
    return -1;

  if (useSem)
    if (!semOpen(SEM_ID, 1)) {
      print(0xFFFFFF, "test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (useSem)
      semWait(SEM_ID);
    slowInc(&global, inc);
    if (useSem)
      semPost(SEM_ID);
  }

  if (useSem)
    semClose(SEM_ID);

  return 0;
}

void testSync(int argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3) {
    print(0xFFFFFF, "Usage: testSync <n> <use_sem>\n");
    return;
  }

  char *argvDec[] = {argv[1], "-1", argv[2], NULL};
  char *argvInc[] = {argv[1], "1", argv[2], NULL};

  global = 0;
  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = createProcess((uint64_t)myProcessInc, 0, 3, argvDec,
                            (char *[]){"tty", "tty"});
    pids[i + TOTAL_PAIR_PROCESSES] = createProcess(
        (uint64_t)myProcessInc, 0, 3, argvInc, (char *[]){"tty", "tty"});
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    waitpid(pids[i]);
    waitpid(pids[i + TOTAL_PAIR_PROCESSES]);
  }

  print(0xFFFFFF, "Final value: %d\n", global);
  return;
}