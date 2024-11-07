// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Drivers/include/videoDriver.h"
#include "include/syscalls.h"
#include "include/keyboardBuffer.h"
#include "include/libasm.h"
#include "include/lib.h"
#include "include/pipe.h"
#include "include/process.h"
#include <naiveConsole.h>
#define MIN(x,y) x < y ? x : y

void sysWrite(int descriptor, const char * str, int len,uint32_t hexColor){
    switch(descriptor){
        case STDOUT:
            if(strcmp( getActiveProcess()->fd[STDOUT], "tty") == 0){
                drawWordLen(hexColor, str, len);
                return;
            } else{
                pipeWrite(getActiveProcess()->fd[STDOUT], str, len);
                return;
            }
        case ERROUT:
            drawWordLen(0x00ff0000, str, len);
        default:
            drawWord(0x00ff0000, "no such descriptor");
        return;
    }

}

int sysRead(int descriptor, char * save, int len){
    if(descriptor != STDIN){
        drawWord(0x00ff0000, "no such descriptor");
    }
    if(strcmp( getActiveProcess()->fd[STDIN], "null") == 0){
        return 0;
    }
    if(strcmp( getActiveProcess()->fd[STDIN], "tty") == 0){

        blockProcess(getActivePid(), READ_STDIN);

        int n=getBufferPosition();
        if(getCharAt(n)==0){
            *save=0;
            return 0;
        }

        int length = MIN(len, getBufferLen());

        for(int i=0; i< length; i++){
            n=getBufferPosition();
            save[i] = getCharAt(n);
            consumeBufferAt(n);
        }
        return length;
    } else{
        return pipeRead(getActiveProcess()->fd[STDIN], save, len);
    }
}

void twoChars(char * first,int j, char * app){
    for(int i = 0; i < 2;i++){
        first[j+i] = app[i];
    }
}
