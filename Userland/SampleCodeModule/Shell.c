#include "include/Shell.h"
#include "include/Buffer.h"
#include "include/lib.h"
#include "include/usrSysCall.h"
#include <stdio.h>
static char buffer[BUFFER_SIZE] = {0};

//void lineRead(char * buffer);

void startingLine(){
    char * startingLine = "$>";
    putString(startingLine);
    clearBuffer();
}

void bufferControl(){
    int i = 0;
    while (i < BUFFER_SIZE) {
        char c = getC();
          
        if(c!=0){
        if (c == '\n'){
            putC(c);
            if (i == 0){
                clearBuffer();
                return;
            }
            buffer[i]=0;
            lineRead(buffer);
            clearBuffer();         //para limpiarlo ahora q lineRead no lo uso
            return;

        }else if (c == '\b'){
            if (i > 0){
                i--;               //borro el ultimo caracter
                putC(c);
            }

        }else{
            if (i < BUFFER_SIZE){
                buffer[i++] = c;   //guardo el caracter en el buffer
                putC(c);
            }
        }
    }}
    return;
}

// Define an array to store command strings
const char *commands[] = {
    "eliminator",
    "time",
    "check Exception",
    "incSize",
    "getRegisters",
    "clear"
};

void lineRead(char *buffer) {
    if (strcmp(buffer, "help") == 0) {
        putString("Las siguientes comandos pueden ser utilizadas: \n");
        
        // Loop through the commands array and print each command
        for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            putString(commands[i]);
            putString("\n");
        }
    }else if (strcmp(buffer, "eliminator") == 0) {
        // Do something for the "eliminator" command
        putString("playing eliminator");
        putString("\n");

    }else if (strcmp(buffer, "time") == 0) {
        char * time;
        call_timeClock(time);
        putString(time);
        putString("\n");
    }else if (strcmp(buffer, "check Exception") == 0) {
        // Do something for the "check" command
        putString("checking exception");
        putString("\n");
    }else if (strcmp(buffer, "incSize") == 0) {
        // Do something for the "incSize" command
        putString("incrementing size");
        putString("\n");
    }else if (strcmp(buffer, "getRegisters") == 0) {
        call_regState();
        putString("\n");
    }else if(strcmp(buffer,"clear")){
    }else{
        putString(buffer);
        putString(":command not found");
        putString("\n");
    }
}

int shellInit() {
    while(1){
        startingLine();
        bufferControl();
    }
    return 0;
}