// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <time.h>
#include <stdint.h>
#include "Drivers/include/keyboardDriver.h"
#include "Drivers/include/videoDriver.h"
#include "Drivers/include/SoundDriver.h"
#include "include/libasm.h"
#include <syscalls.h>
#include <naiveConsole.h>
#include "include/interrupts.h"
#include "include/scheduler.h"
#include "include/process.h"
#include "include/lib.h"
#include "include/registerHandling.h"
#include "mm/mm.h"
#include "tests/test_util.h"
#include "include/irqDispatcher.h"
static void int_21();
static int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
void irqDispatcher(uint64_t irq,uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
	switch (irq) {
		case 0:
            timer_handler();
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

void int_21() {
	keyboard_handler();
}
static int int_80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9){
    switch(rdi) {
        case 1:
            sys_write(rsi, (char *)rdx, rcx,r8);
            return 0;
        case 2:
            return sys_read(rsi,(char *) rdx, rcx);
        case 3:
            clock((char *) rsi);
            return 0;
        case 4:
            if(getFlag() || rsi == 1){
                if(getFlag()==0){
                    drawWord(0x00FF0000,"You must take a screenshot first, press : and try again.\n");
                    return 0;
                }
                printRegisters(getRegisters(), 0x00ffffff);
            }
            //la idea faltaria que se prenda al pedir registros
            return 0;
        case 5:
            clear();
            return 0;
        case 6:
            return ticks_elapsed();
        case 7:
           return (getHeight());
        case 8:
           return (getWidth());
        case 9:
            moveCursorX((uint16_t)rsi);
            return 0;
        case 10:
            moveCursorY((uint16_t)rsi);
            return 0;
        case 11:
            drawRectangle(rsi, rdx, rcx, r8, r9);
            return 0;
        case 12:
            sleepms(rsi);
            return 0;
        case 13:
            return (int) setFontSize(rsi);
        case 14:
            beep();
            return 0;
        case 15:
            runTestMm();
           return 0;
        case 16:
            printMm();
            return 0;
        case 17:
            char aux[10];
            intToStr(getActivePid(),aux);
           // drawWord(0xFFFFFFFF,aux);
            killProcess(getActivePid());
            yield();
            return 0;
        case 18:
           return newProcess(rsi,(int)rdx,(int)rcx,(int)r8,(char **)r9);
        case 19:
            return  killProcess((pid_t) rsi);
        case 20:
            return  blockProcess((pid_t) rsi);
        case 21:
            return  unblockProcess((pid_t) rsi);
        case 22:
            return (uint64_t) allocMemory((size_t)rsi);
        case 23:
             freeMemory((void *)rsi);
             return 0;
        case 24:
            return changePrio((pid_t) rsi,(int) rdx);
        case 25:
            return getActivePid();
        case 26:
            newProcess((uint64_t)testeando,0,1,0,NULL);
            return 0;
        default:

            return 0;
    }
}


