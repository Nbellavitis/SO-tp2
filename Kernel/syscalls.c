//
// Created by nicol on 5/22/2024.
//
#include "Drivers/include/videoDriver.h"
#include "include/syscalls.h"
#include "include/keyboardBuffer.h"
#include <naiveConsole.h>
#include <stdio.h>
#define MIN(x,y) x < y ? x : y

void sys_write(int descriptor, char * str, int len){
    switch(descriptor){
        case STDOUT:
            drawWordLen(0x00ffffff, str, len);
            return;
        case ERROUT:
            drawWordLen(0x00ff0000, str, len);
        default: // poner error aca
        return;
    }

}

void sys_read(int descriptor, char * save, int len){
    
    if(descriptor != STDIN){
        // ERROR
    }
   
    int n=getBufferPosition();
 
    if(getCharAt(n)==0){
        *save=0;
        return;
    }
    
    int length = MIN(len, getBufferLen());

    for(int i=0; i< length; i++){
        n=getBufferPosition();
        save[i] = getCharAt(n);
        consumeBufferAt(n);
    }
}



