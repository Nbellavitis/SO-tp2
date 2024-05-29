#include "stdint.h"

#ifndef USERSYSCALLS_H
#define USERSYSCALLS_H

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int call_sys_read(int fd,char *buf, int size);
void call_sys_write(int fd,char *buf, int size,uint32_t hexColor);
void call_timeClock(char *buf);
void call_printRegisters(int flag);
void call_drawRectangle(uint64_t color, uint32_t x, uint32_t y, uint32_t height, uint32_t width);
int call_getBufferPosition();
int call_getCharAt(int pos);
void call_put_square(uint32_t x, uint32_t y, uint32_t size, uint64_t hexColor);
void call_sleepms(int mseconds);
void call_setFontSize(uint32_t size);
void call_drawWordColorAt(uint64_t hexColor, char* word, uint32_t x, uint32_t y);
void call_characterAt(uint64_t hexColor, char c, uint32_t x, uint32_t y);
void call_beep();
void call_clear();
void InvalidOpasm();
uint16_t call_getWidth();
uint16_t call_getHeight();
void call_moveCursorY(uint16_t posY);
void call_moveCursorX(uint16_t posX);
#endif /* USERSYSCALLS_H */