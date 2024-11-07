#include "stdint.h"
#include "lib.h"
#include "../../../sharedUserKernel.h"
#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H

#define STDIN 0
#define STDOUT 1
#define STDERR 2

// Reads data from a file descriptor into the kernels buffer
int callSysRead(int fd, char *buf, int size);

// Writes data from the  kernels buffer to a file descriptor with a specified color
void callSysWrite(int fd, const char *buf, int size, uint32_t hexColor);

// Gets the current time and stores it in a buffer
void callTimeClock(char *buf);

// Prints registers based on a flag value
void callPrintRegisters(int flag);

// Draws a rectangle on the screen with specified color, position, and dimensions
void callDrawRectangle(uint64_t color, uint32_t x, uint32_t y, uint32_t height, uint32_t width);

// Returns the current position of the kernels buffer
int callGetBufferPosition();

// Gets the character at a specified position on the kernels buffer
int callGetCharAt(int pos);

// Draws a squarewith specified position, size, and color
void callPutSquare(uint32_t x, uint32_t y, uint32_t size, uint64_t hexColor);

// Pauses the program execution for a specified number of milliseconds
void callSleepMs(int mseconds);

// Sets the font size for text 
int callSetFontSize(uint32_t size);

// Draws a word on a graphics screen with specified color at a given position
void callDrawWordColorAt(uint64_t hexColor, char* word, uint32_t x, uint32_t y);

// Draws a character  with specified color at a given position
void callCharacterAt(uint64_t hexColor, char c, uint32_t x, uint32_t y);

// Produces a beep 
void callBeep();

// Clears the screen 
void callClear();

// Handles an invalid operation in assembly language
void invalidOpAsm();

// Gets the width of the screen or drawing area
uint16_t callGetWidth();

// Gets the height of the screen or drawing area
uint16_t callGetHeight();

// Moves the cursor to a specified vertical position
void callMoveCursorY(uint16_t posY);

// Moves the cursor to a specified horizontal position
void callMoveCursorX(uint16_t posX);

// Gets the elapsed time since some reference point in milliseconds
int callGetTicks();
int killProcess(pid_t pid );
int blockProcess(pid_t pid );
int unblockProcess(pid_t pid );
void changePrio(pid_t pid, int prio);
pid_t getMyPid();
void * allocMemory(size_t size);
void  freeMemory(void * mem);
pid_t createProcess(uint64_t rip, int ground, int argc, char * argv[], char * fd[2]);
void testMM();
void mmStatus();
void testeando();
processInfoPtr * ps();
long int waitpid(pid_t pid);
int semOpen(char * name, int initialValue);
void semClose(char * name);
void semWait(char * name);
void semPost(char * name);
void yield();
int pipeOpen(char * name);
int pipeWrite(char * name, const char * str, int len);
int pipeRead(char * name, char * str, int len);
int pipeClose(char * name);
int pipeOpenAnon(char * name);
char ** getMyFds();
#endif /* USERSYSCALLS_H */