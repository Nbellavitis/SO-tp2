#include <time.h>
#include <stdint.h>
#include "Drivers/include/keyboardDriver.h"
#include "Drivers/include/videoDriver.h"
#include "Drivers/include/SoundDriver.h"
#include "include/libasm.h"
#include "include/lib.h"
#include <syscalls.h>
#include <naiveConsole.h>
#include "include/interrupts.h"
#include "include/lib.h"
#include "include/registerHandling.h"
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
            if(getFlag() || rsi == 1){
                printRegisters(getRegisters(), 0x00ffffff);
            }
            //la idea faltaria que se prenda al pedir registros
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
        case 11:
            drawRectangle(rsi, rdx, rcx, r8, r9);
            return;
        case 12:
            sleepms(rsi);
            return;
        case 13:
            setFontSize(rsi);
            return;
        case 14:
            beep();
        case 15:
            ticks_elapsed();
        default:
            return;
    }
}


