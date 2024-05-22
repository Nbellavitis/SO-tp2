#include <time.h>
#include <stdint.h>
#include "Drivers/include/keyboardDriver.h"
#include <syscalls.h>
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
            sys_write(rsi, (char *)rdx, rcx);
            return;
        case 2:
            sys_read(rsi,(char *) rdx, rcx);
            return;
        default:
            return;
    }
}


