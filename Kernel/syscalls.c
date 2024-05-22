//
// Created by nicol on 5/22/2024.
//
#include "drivers/include/videoDriver.h"
#include "syscalls.h"
void sys_write(int descriptor, char * str, int len){
    switch(descriptor){
        case STDOUT:
            drawWordLen(0x00ffffff, str, len);
            return;
        case ERROUT:
            drawWordLen(0x00ff0000, str, len);
        default: // poner error aca
    }

}