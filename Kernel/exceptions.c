// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#include "Drivers/include/videoDriver.h"
#include "include/registerHandling.h"
#include "include/syscalls.h"
#include "include/interrupts.h"
static void zeroDivision();
static void invalidOpcode();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID){
		zeroDivision();
	}else{
		invalidOpcode();
		}
    printRegisters(getRegisters(), 0x00ff0000);
    drawWord(0x00ff0000,"Press any key to continue");
    while(1) {
        char c;
        _hlt();
        sysRead(STDIN, &c, 1); // no se si esta bien usarlo asi
        if (c != 0) {
            clear();
            return;
        }
    }

}

static void zeroDivision() {
	drawWord(0x00FF0000, "Cannot divide by 0");
	newLine();
    return;
}

static void invalidOpcode(){

	drawWord(0x00FF0000, "Error:Invalid opcode");
	newLine();

}