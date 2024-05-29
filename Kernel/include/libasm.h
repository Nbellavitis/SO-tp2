#ifndef lisasm_h
#define lisasm_h
#include <stdint.h>
char * getMinutes();
char * getSeconds();
char * getHours();
uint8_t inb(uint8_t);
void outb(uint8_t,uint8_t);
#endif