
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6
#include "Drivers/include/videoDriver.h"
#include "include/registerHandling.h"
static void zero_division();
static void invalid_opcode();

void exceptionDispatcher(int exception) {
    drawWord(0x00ff0000,"hola");
	if (exception == ZERO_EXCEPTION_ID){
		zero_division();
	}else{
		invalid_opcode();
		}
	//printRegisters();
}

static void zero_division() {
	drawWord(0x00FF0000, "No se puede dividir por 0");
	newLine();
    return;
}

static void invalid_opcode(){

	drawWord(0x00FF0000, "Error:Invalid opcode");
	newLine();

}