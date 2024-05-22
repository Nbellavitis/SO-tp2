#include <time.h>
#include <stdint.h>
#include "Drivers/include/keyboardDriver.h"
static void int_20();
static void int_21();
static void int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
        case 0x80:
            int_80();
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
            sys_write(rsi, rdx, rcx);
            return;
        case 2:
            sys_read(rsi, rdx, rcx);
            return;
        default:
            return;
    }
}


