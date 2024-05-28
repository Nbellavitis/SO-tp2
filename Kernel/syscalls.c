//
// Created by nicol on 5/22/2024.
//
#include "Drivers/include/videoDriver.h"
#include "include/syscalls.h"
#include "include/keyboardBuffer.h"
#include "include/libasm.h"
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

void twoChars(char * first,int j, char * app){
    for(int i = 0; i < 2;i++){
        first[j+i] = app[i];
    }
}

void sys_readTime(char * time){
    char hours[3];
    char minutes[3];
    char seconds[3];
    twoChars(time,0,hours);
    time[2] = ':';
    twoChars(time,3,minutes);
    time[5] = ':';
    twoChars(time,6,seconds);
    
}



void printTitle(){
    drawWordAt(0X00FF0000,"ELIMINATOR",getWidth()/2-5*8,getHeight()/2);
}