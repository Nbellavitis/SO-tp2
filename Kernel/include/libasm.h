#ifndef lisasm_h
#define lisasm_h
#include <stdint.h>
//Returns minutes in gmt
char * getMinutes();
//Returns seconds in gmt
char * getSeconds();
//Returns hours in gmt
char * getHours();
uint8_t inb(uint8_t);
void outb(uint8_t,uint8_t);
uint16_t getKey();
#endif