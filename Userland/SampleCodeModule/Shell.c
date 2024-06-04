#include "include/Shell.h"
#include "include/Buffer.h"
#include "include/lib.h"
#include "include/usrSysCall.h"
#include "include/eliminator.h"
#include <stdio.h>
#define WHITE 0xFFFFFFFF
static char buffer[BUFFER_SIZE] = {0};
int exitFlag =0;
int registerFlag = 0;

void lineRead(char * buffer);
void call_InvalidOp();
void reSize(char * buffer);
void call_div0();

void startingLine(){
    char * startingLine = "$>";
    putString(startingLine,GREEN);
    clearBuffer(buffer);
}

void bufferControl(){
    int i = 0;
    while (1) {
        char c;
         getC(&c);
          
        if(c!=0){
        if (c == '\n'){
            putC(c,WHITE);
            if (i == 0){
                clearBuffer(buffer);
                return;
            }
            buffer[i]=0;
            lineRead(buffer);
            clearBuffer(buffer);         //para limpiarlo ahora q lineRead no lo uso
            return;

        }else if (c == '\b'){
            if (i > 0){
                i--;               //borro el ultimo caracter
                putC(c,WHITE);
            }

        }else{
            if (i < BUFFER_SIZE){
                buffer[i++] = c;   //guardo el caracter en el buffer
                putC(c,WHITE);
            }
        }
    }

    }
    return;
}

// Define an array to store command strings
const char *commands[] = {
    "eliminator:   Game similar to tron(the movie).",
    "time:         Shows the actual time.",
    "setFont:      Change the font size, receive an int from 1 to 2.",
    "getRegisters: Show the actual state of the registers.",
    "clear:        Empty the terminal.",
    "exit:         Kills the terminal.",
    "div0:         Test the exception of the cero division.",
    "invalidOp:    Test the exception of an invalid operand."
};

void lineRead(char *buffer) {
    if (strcmp(buffer, "help") == 0) {
        putString("The following commands may be used: \n",WHITE);
        for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            putString(commands[i],WHITE);
            putString("\n",WHITE);
        }
    }else if (strcmp(buffer, "eliminator") == 0) {
      startEliminator();

    }else if (strcmp(buffer, "time") == 0) {
        char time[9];                               //Viene dada por el formato hh:mm:ss por eso son 8 mas la terminacion en cero
        call_timeClock(time);
        putString(time,WHITE);
        putString("\n",WHITE);
    }else if (strcmpSpace(buffer, "setFont") == 0) {
        call_clear();
        reSize(buffer);
        clearBuffer(buffer);
    }else if (strcmp(buffer, "getRegisters") == 0) {
        call_printRegisters(1);
    }else if(strcmp(buffer,"clear")==0){
        call_clear();
        clearBuffer(buffer);
    }else if(strcmp(buffer,"exit")==0){
        exitFlag=1;
         call_clear();
        clearBuffer(buffer);
        return;
    }else if(strcmp(buffer,"div0")==0){
        call_div0();
        return;
    }else if(strcmp(buffer,"invalidOp")==0){
        call_InvalidOp();
        return;
    }else{
        putString(buffer,WHITE);
        putString(":command not found",WHITE);
        putString("\n",WHITE);
    }
    call_printRegisters(0);
 
}

void reSize(char * buffer){
    char * init = buffer + strlen("setFont ");
    call_setFontSize(strToInt(init));
}


int shellInit() {
    char * start = "Welcome to Jungle OS, type help to get a list of commands.\n";
    putString(start,GREEN);
    clearBuffer(buffer);
    while(!exitFlag){
        startingLine();
        bufferControl();
    }
    return 0;
}
void call_InvalidOp(){
    InvalidOpasm();
}
void call_div0(){
    int a=7;
    int b=0;
    a=a/b;
}