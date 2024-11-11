#include "../../../sharedUserKernel.h"
#include "lib.h"
#include "stdint.h"
#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int callSysRead(int fd, char *buf, int size);
void callSysWrite(int fd, const char *buf, int size, uint32_t hexColor);
void callTimeClock(char *buf);
void callPrintRegisters(int flag);
void callDrawRectangle(uint64_t color, uint32_t x, uint32_t y, uint32_t height,
                       uint32_t width);
int callGetBufferPosition();
int callGetCharAt(int pos);
void callPutSquare(uint32_t x, uint32_t y, uint32_t size, uint64_t hexColor);
void callSleepMs(int mseconds);
int callSetFontSize(uint32_t size);
void callDrawWordColorAt(uint64_t hexColor, char *word, uint32_t x, uint32_t y);
void callCharacterAt(uint64_t hexColor, char c, uint32_t x, uint32_t y);
void callBeep();
void callClear();
void invalidOpAsm();
uint16_t callGetWidth();
uint16_t callGetHeight();
void callMoveCursorY(uint16_t posY);
void callMoveCursorX(uint16_t posX);
int callGetTicks();
int killProcess(pid_t pid);
int blockProcess(pid_t pid);
int unblockProcess(pid_t pid);
void changePrio(pid_t pid, int prio);
pid_t getMyPid();
void *allocMemory(size_t size);
void freeMemory(void *mem);
pid_t createProcess(uint64_t rip, int ground, int argc, char *argv[],
                    char *fd[2]);
void mmStatus();
void testeando();
processInfoPtr *ps();
long int waitpid(pid_t pid);
int semOpen(char *name, int initialValue);
void semClose(char *name);
void semWait(char *name);
void semPost(char *name);
void yield();
int pipeOpen(char *name);
int pipeWrite(char *name, const char *str, int len);
int pipeRead(char *name, char *str, int len);
int pipeClose(char *name);
int pipeOpenAnon(char *name);
char **getMyFds();
#endif /* USERSYSCALLS_H */