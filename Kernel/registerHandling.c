#include "include/libasm.h"
#include <stdint.h>
#include "include/registerHandling.h"
#include "Drivers/include/videoDriver.h"
#define SIZE 17
char* register_names[SIZE] = {"rip", "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp","r8 ", "r9 ", "r10", "r11", "r12", "r13", "r14", "r15"};
    

    
void printRegisters(uint64_t * registers,uint32_t colour){
    for (int i = 0; i < SIZE; ++i) {
        drawWord(colour,register_names[i]);
        drawWord(colour,": 0x");
        drawRegister(registers[i],colour);
    };
}