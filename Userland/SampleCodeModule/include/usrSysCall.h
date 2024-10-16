#include "stdint.h"
#include "lib.h"
#include "../../../sharedUserKernel.h"
#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H

#define STDIN 0
#define STDOUT 1
#define STDERR 2

// Reads data from a file descriptor into the kernels buffer
int call_sys_read(int fd, char *buf, int size);

// Writes data from the  kernels buffer to a file descriptor with a specified color
void call_sys_write(int fd, const char *buf, int size, uint32_t hexColor);

// Gets the current time and stores it in a buffer
void call_timeClock(char *buf);

// Prints registers based on a flag value
void call_printRegisters(int flag);

// Draws a rectangle on the screen with specified color, position, and dimensions
void call_drawRectangle(uint64_t color, uint32_t x, uint32_t y, uint32_t height, uint32_t width);

// Returns the current position of the kernels buffer
int call_getBufferPosition();

// Gets the character at a specified position on the kernels buffer
int call_getCharAt(int pos);

// Draws a squarewith specified position, size, and color
void call_put_square(uint32_t x, uint32_t y, uint32_t size, uint64_t hexColor);

// Pauses the program execution for a specified number of milliseconds
void call_sleepms(int mseconds);

// Sets the font size for text 
int call_setFontSize(uint32_t size);

// Draws a word on a graphics screen with specified color at a given position
void call_drawWordColorAt(uint64_t hexColor, char* word, uint32_t x, uint32_t y);

// Draws a character  with specified color at a given position
void call_characterAt(uint64_t hexColor, char c, uint32_t x, uint32_t y);

// Produces a beep 
void call_beep();

// Clears the screen 
void call_clear();

// Handles an invalid operation in assembly language
void InvalidOpasm();

// Gets the width of the screen or drawing area
uint16_t call_getWidth();

// Gets the height of the screen or drawing area
uint16_t call_getHeight();

// Moves the cursor to a specified vertical position
void call_moveCursorY(uint16_t posY);

// Moves the cursor to a specified horizontal position
void call_moveCursorX(uint16_t posX);

// Gets the elapsed time since some reference point in milliseconds
int call_getTicks();
int killProcess(pid_t pid );
int blockProcess(pid_t pid );
int unblockProcess(pid_t pid );
void changePrio(pid_t pid, int prio);
pid_t getMyPid();
void * allocMemory(size_t size);
void  freeMemory(void * mem);
pid_t createProcess(uint64_t rip, int ground, int priority, int argc, char * argv[]);
void testmm();
void mmStatus();
void testeando();
processInfoPtr * ps();
long int waitpid(pid_t pid);
#endif /* USERSYSCALLS_H */