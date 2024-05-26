#include "include/Shell.h"
#include "include/Buffer.h"
#include "include/lib.h"
#include "include/usrSysCall.h"
#include <stdio.h>
static char buffer[BUFFER_SIZE] = {0};
int exitFlag =0;

//void lineRead(char * buffer);

void startingLine(){
    char * startingLine = "$>";
    putString(startingLine);
    clearBuffer();
}

void bufferControl(){
    int i = 0;
    while (1) {
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
    }

    }
    return;
}

// Define an array to store command strings
const char *commands[] = {
    "eliminator",
    "time",
    "check Exception",
    "setFont",
    "getRegisters",
    "clear",
    "exit"
};

void lineRead(char *buffer) {
    if (strcmp(buffer, "help") == 0) {
        putString("Las siguientes comandos pueden ser utilizadas: \n");
        for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            putString(commands[i]);
            putString("\n");
        }
    }else if (strcmp(buffer, "eliminator") == 0) {
        putString("playing eliminator");
        putString("\n");

    }else if (strcmp(buffer, "time") == 0) {
        char time[9];//Viene dada por el formato hh:mm:ss por eso son 8 mas la terminacion en cero
        //call_timeClock(time);
       //putString(time);
        putString("\n");
    }else if (strcmp(buffer, "check Exception") == 0) {
        putString("checking exception");
        putString("\n");
    }else if (strcmp(buffer, "setFont") == 0) {
        int size;
        call_clear();
        clearBuffer();
        //falta hacer un scan para guardar el size
        //se podria hacer buffer control y en vez de line read se para como parametro una funcion ene este caso scanf
        call_setFontSize();
    }else if (strcmp(buffer, "getRegisters") == 0) {
        call_printRegisters();
    }else if(strcmp(buffer,"clear")==0){
        call_clear();
        clearBuffer();
    }else if(strcmp(buffer,"exit")==0){
        exitFlag=1;
        //Limpiaria la pantalla aca
        return;
    }else{
        putString(buffer);
        putString(":command not found");
        putString("\n");
    }
}


int shellInit() {
    while(!exitFlag){
        startingLine();
        bufferControl();
    }
    return 0;
}