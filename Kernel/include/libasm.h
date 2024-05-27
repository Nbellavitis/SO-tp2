#ifndef lisasm_h
#define lisasm_h
#include <stdint.h>
char * getMinutes();
char * getSeconds();
char * getHours();
void getRegisters(uint64_t * regs);
#endif