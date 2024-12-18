// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "include/registerHandling.h"
#include "Drivers/include/videoDriver.h"
#include "include/libasm.h"
#include <stdint.h>
#define SIZE 18
char *registerNames[SIZE] = {"rflags", "rsp", "rip", "rbp", "rax", "rbx",
                             "rcx",    "rdx", "rdi", "rsi", "r8 ", "r9 ",
                             "r10",    "r11", "r12", "r13", "r14", "r15"};

void printRegisters(uint64_t *registers, uint32_t colour) {
  for (int i = 0; i < SIZE; i++) {
    drawWord(colour, registerNames[i]);
    drawWord(colour, ": 0x");
    drawRegister(registers[SIZE - i - 1], colour);
  }
}