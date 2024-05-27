#include "include/libasm.h"
#include "../Userland/SampleCodeModule/include/lib.h"

#define SIZE 17
const char register_names[SIZE] = {
        "rip", "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp",
        "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
        
    };
    
void printRegisters(){
    uint64_t registers[SIZE];
    call_getRegisters(registers);
    for (int i = 0; i < SIZE; ++i) {
        print("%s: 0x%x\n", register_names[i], registers[i]);
    }
}