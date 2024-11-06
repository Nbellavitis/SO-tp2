#include "include/keyboardDriver.h"
#include "../include/keyboardBuffer.h"
#include "../include/interrupts.h"
#include "include/scanCode.h"
#include "include/videoDriver.h"
#include "../include/scheduler.h"
#include "../include/libasm.h"
#define EOF -1
int keyMapRow = 0;
int ctrlPressed = 0;

uint8_t scancodeToAscii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '|',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t scancodeShiftToAscii[] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
    '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t *keyMap[] = {scancodeToAscii, scancodeShiftToAscii};


void keyboardHandler() {
    uint16_t code = getKey();
    if (code < 0x80) { // Key pressed
        char *buff = getBufferAddress();
        int buff_pos = getBufferPosition();
        if (code == LEFT_SHIFT || code == RIGHT_SHIFT) {
            keyMapRow = 1;
        } else if (code == LEFT_CTRL || code == RIGHT_CTRL) {
            ctrlPressed = 1;
        } else if (ctrlPressed && keyMap[keyMapRow][code] == 'c') {
         PCB aux = getCurrentForegroundProcess();
         if(aux != NULL && aux->pid != 1){
             killProcess(aux->pid);
          }
            drawWord(0x00FF0000,"Process killed\n");
        } else if(ctrlPressed && keyMap[keyMapRow][code] == 'd'){
            buff[buff_pos] = EOF;
            incBufferLen(1);
            setPos(buff_pos);
        }
        else if (keyMap[keyMapRow][code] != 0) {
            buff[buff_pos] = keyMap[keyMapRow][code];
            incBufferLen(1);
            setPos(buff_pos);
        }
    } else { // Key released
        code -= 0x80;
        if (code == LEFT_SHIFT || code == RIGHT_SHIFT) {
            keyMapRow &= 0xFE;
        } else if (code == LEFT_CTRL || code == RIGHT_CTRL) {
            ctrlPressed = 0;
        }
    }
    PCB aux = getCurrentForegroundProcess();
    if (aux != NULL && aux->pid != 1) {
        if (aux->waitingFor == READ_STDIN) {
            unblockProcess(aux->pid);
        }
    } else {
        PCB shell = lookUpOnHashMap(1);
        if (shell->waitingFor == READ_STDIN) {
            unblockProcess(1);
        }
    }

    return;
}