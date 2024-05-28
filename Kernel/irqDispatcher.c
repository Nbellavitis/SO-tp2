#include <time.h>
#include <stdint.h>
#include "Drivers/include/keyboardDriver.h"
#include "Drivers/include/videoDriver.h"
#include "include/libasm.h"
#include <syscalls.h>
#include <naiveConsole.h>
#include "include/interrupts.h"
static void int_20();
static void int_21();
static void int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
void irqDispatcher(uint64_t irq,uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
        case 0x80:
            int_80(rdi, rsi,  rdx, rcx, r8, r9);
            break;
        }
	return;
}

void int_20() {
	timer_handler();
}
void int_21() {
	keyboard_handler();
}
static void int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    switch(rdi) {
        case 1:
            sys_write(rsi, (char *)rdx, rcx,r8);
            return;
        case 2:
            sys_read(rsi,(char *) rdx, rcx);
            return;
        case 3:
            clock((char *) rsi);
            return;
        case 4:
            printRegistersAsm(0x00FFFFFF);
            return;
        case 5:
            clear();
            return;
        case 6:
            printTitle();
            return;
        case 7:
           return (getHeight());
        case 8:
           return (getWidth());
        case 9:
            moveCursorX((uint16_t)rsi);
            return; 
        case 10:
            moveCursorY((uint16_t)rsi);
            return;
        case 13:
            setFontSize(rsi);
            return;
        default:
            return;
    }
}


