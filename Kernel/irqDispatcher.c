#include "include/irqDispatcher.h"
#include "Drivers/include/keyboardDriver.h"
#include "Drivers/include/soundDriver.h"
#include "Drivers/include/videoDriver.h"
#include "include/interrupts.h"
#include "include/lib.h"
#include "include/libasm.h"
#include "include/pipe.h"
#include "include/process.h"
#include "include/registerHandling.h"
#include "include/scheduler.h"
#include "include/sems.h"
#include "mm/mm.h"
#include "tests/testUtil.h"
#include <naiveConsole.h>
#include <stdint.h>
#include <syscalls.h>
#include <time.h>

static void int21();
static int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                 uint64_t r8, uint64_t r9);
void timerHandler();

void irqDispatcher(uint64_t irq, uint64_t rdi, uint64_t rsi, uint64_t rdx,
                   uint64_t rcx, uint64_t r8, uint64_t r9) {
  switch (irq) {
  case 0:
    timerHandler();
    break;
  case 1:
    int21();
    break;
  case 0x80:
    int80(rdi, rsi, rdx, rcx, r8, r9);
    break;
  }
  return;
}

void int21() { keyboardHandler(); }

typedef int (*syscallHandler_t)(uint64_t, uint64_t, uint64_t, uint64_t,
                                uint64_t, uint64_t);

static int sysWriteWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t r9) {
  sysWrite(rsi, (char *)rdx, rcx, r8);
  return 0;
}

static int sysReadWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                          uint64_t rcx, uint64_t r8, uint64_t r9) {

  return sysRead(rsi, (char *)rdx, rcx);
}

static int clockWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                        uint64_t r8, uint64_t r9) {
  clock((char *)rsi);
  return 0;
}

static int clearWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                        uint64_t r8, uint64_t r9) {
  clear();
  return 0;
}

static int ticksElapsedWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                               uint64_t rcx, uint64_t r8, uint64_t r9) {
  return ticksElapsed();
}

static int getHeightWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                            uint64_t rcx, uint64_t r8, uint64_t r9) {
  return getHeight();
}

static int getWidthWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t r9) {
  return getWidth();
}

static int moveCursorXWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                              uint64_t rcx, uint64_t r8, uint64_t r9) {
  moveCursorX((uint16_t)rsi);
  return 0;
}

static int moveCursorYWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                              uint64_t rcx, uint64_t r8, uint64_t r9) {
  moveCursorY((uint16_t)rsi);
  return 0;
}

static int drawRectangleWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                                uint64_t rcx, uint64_t r8, uint64_t r9) {
  drawRectangle(rsi, rdx, rcx, r8, r9);
  return 0;
}

static int sleepmsWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                          uint64_t rcx, uint64_t r8, uint64_t r9) {
  sleepms(rsi);
  return 0;
}

static int setFontSizeWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                              uint64_t rcx, uint64_t r8, uint64_t r9) {
  return setFontSize(rsi);
}

static int beepWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                       uint64_t r8, uint64_t r9) {
  beep();
  return 0;
}

static int runTestMemWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                             uint64_t rcx, uint64_t r8, uint64_t r9) {
  runTestMm();
  return 0;
}

static int printMemWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t r9) {
  printMm();
  return 0;
}

static int exitProcessWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                              uint64_t rcx, uint64_t r8, uint64_t r9) {
  exitProcess((pid_t)rsi);
  return 0;
}

static int newProcessWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                             uint64_t rcx, uint64_t r8, uint64_t r9) {
  return newProcess(rsi, rdx, 1, rcx, (char **)r8, (char **)r9);
}

static int killProcessWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                              uint64_t rcx, uint64_t r8, uint64_t r9) {
  return killProcess((pid_t)rsi);
}

static int blockProcessWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                               uint64_t rcx, uint64_t r8, uint64_t r9) {
  return blockProcess((pid_t)rsi, CHILD_PROCESS);
}

static int unblockProcessWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                                 uint64_t rcx, uint64_t r8, uint64_t r9) {
  return unblockProcess((pid_t)rsi);
}

static int allocMemoryWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                              uint64_t rcx, uint64_t r8, uint64_t r9) {
  return (uint64_t)allocMemory((uint64_t)rsi);
}

static int freeMemoryWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                             uint64_t rcx, uint64_t r8, uint64_t r9) {
  freeMemory((void *)rsi);
  return 0;
}

static int changePriorityWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                                 uint64_t rcx, uint64_t r8, uint64_t r9) {
  return changePrio((pid_t)rsi, (int)rdx);
}

static int getActivePidWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                               uint64_t rcx, uint64_t r8, uint64_t r9) {
  return getActivePid();
}

static int waitpidWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                          uint64_t rcx, uint64_t r8, uint64_t r9) {
  return waitpid((pid_t)rsi);
}

static int getAllProcessInfoWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                                    uint64_t rcx, uint64_t r8, uint64_t r9) {
  return (int64_t)getAllProcessInfo();
}

static int getRegistersStateWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                                    uint64_t rcx, uint64_t r8, uint64_t r9) {
  if (getFlag() || rsi == 1) {
    if (getFlag() == 0) {
      drawWord(0x00FF0000,
               "You must take a screenshot first, press : and try again.\n");
      return 0;
    }
    printRegisters(getRegisters(), 0x00ffffff);
  }
  return 0;
}

static int yieldWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                        uint64_t r8, uint64_t r9) {
  yield();
  return 0;
}
static int testeandoWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                            uint64_t rcx, uint64_t r8, uint64_t r9) {
  testeando();
  return 0;
}
static int semOpenWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                          uint64_t rcx, uint64_t r8, uint64_t r9) {
  return semOpen((char *)rsi, rdx);
}

static int semCloseWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t r9) {
  semClose((char *)rsi);
  return 0;
}
static int semWaitWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                          uint64_t rcx, uint64_t r8, uint64_t r9) {
  semWait((char *)rsi);
  return 0;
}
static int semPostWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                          uint64_t rcx, uint64_t r8, uint64_t r9) {
  semPost((char *)rsi);
  return 0;
}
static int pipeOpenWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t r9) {
  return pipeOpen((char *)rsi);
}
static int pipeWriteWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                            uint64_t rcx, uint64_t r8, uint64_t r9) {
  return pipeWrite((char *)rsi, (char *)rdx, rcx);
}
static int pipeReadWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t r9) {
  return pipeRead((char *)rsi, (char *)rdx, rcx);
}
static int pipeCloseWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                            uint64_t rcx, uint64_t r8, uint64_t r9) {
  return pipeClose((char *)rsi);
}
static int pipeOpenAnonWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                               uint64_t rcx, uint64_t r8, uint64_t r9) {
  return pipeOpenAnon((char *)rsi);
}
static int getMyFdsWrapper(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rcx, uint64_t r8, uint64_t r9) {
  return (int64_t)getMyFds();
}
syscallHandler_t syscallHandlers[] = {
    NULL,                     // 0 (reservado)
    sysWriteWrapper,          // 1
    sysReadWrapper,           // 2
    clockWrapper,             // 3
    getRegistersStateWrapper, // 4
    clearWrapper,             // 5
    ticksElapsedWrapper,      // 6
    getHeightWrapper,         // 7
    getWidthWrapper,          // 8
    moveCursorXWrapper,       // 9
    moveCursorYWrapper,       // 10
    drawRectangleWrapper,     // 11
    sleepmsWrapper,           // 12
    setFontSizeWrapper,       // 13
    beepWrapper,              // 14
    runTestMemWrapper,        // 15
    printMemWrapper,          // 16
    exitProcessWrapper,       // 17
    newProcessWrapper,        // 18
    killProcessWrapper,       // 19
    blockProcessWrapper,      // 20
    unblockProcessWrapper,    // 21
    allocMemoryWrapper,       // 22
    freeMemoryWrapper,        // 23
    changePriorityWrapper,    // 24
    getActivePidWrapper,      // 25
    testeandoWrapper,         // 26
    getAllProcessInfoWrapper, // 27
    waitpidWrapper,           // 28
    yieldWrapper,             // 29
    semOpenWrapper,           // 30
    semWaitWrapper,           // 31
    semPostWrapper,           // 32
    semCloseWrapper,          // 33
    pipeOpenWrapper,          // 34
    pipeWriteWrapper,         // 35
    pipeReadWrapper,          // 36
    pipeCloseWrapper,         // 37
    pipeOpenAnonWrapper,      // 38
    getMyFdsWrapper           // 39
};

// Función int_80 utilizando el array de syscalls
static int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                 uint64_t r8, uint64_t r9) {
  if (rdi < sizeof(syscallHandlers) / sizeof(syscallHandlers[0]) &&
      syscallHandlers[rdi] != NULL) {
    return syscallHandlers[rdi](rdi, rsi, rdx, rcx, r8, r9);
  }
  return 0;
}