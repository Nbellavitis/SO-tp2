#include "include/libasm.h"
#include <stdint.h>
#include "Drivers/include/videoDriver.h"
#define SIZE 17
char* register_names[SIZE] = {"rip", "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp","r8 ", "r9 ", "r10", "r11", "r12", "r13", "r14", "r15"};
    

    
void printRegisters(){
    uint64_t registers[SIZE];
    getRegisters(registers);
    for (int i = 0; i < SIZE; ++i) {
        drawWord(0xFFFFFFFF,register_names[i]);
        drawWord(0xFFFFFFFF,": 0x");
        drawRegister(registers[i]);
    };
}