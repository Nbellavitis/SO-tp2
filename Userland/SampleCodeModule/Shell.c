#include "include/Shell.h"
#include "include/Buffer.h"
#include "include/lib.h"

static char buffer[BUFFER_SIZE] = {0};

//void lineRead(char * buffer);

void startingLine(){
    char * startingLine = "$>";
    print(startingLine);
    clearBuffer();
}

void bufferControl(){
    int i = 0;
    while (i < BUFFER_SIZE) {
        char c = getC();

        if (c == '\n'){
            putC(c);
            if (i == 0){
                clearBuffer();
                return;
            }
            buffer[i]=0;
            //lineRead(buffer);
            clearBuffer();         //para limpiarlo ahora q lineRead no lo uso
            return;

        }else if (c == '\b'){
            if (i > 0){
                i--;               //borro el ultimo caracter
            }
            putC(c);

        }else{
            if (i != BUFFER_SIZE-1){
                buffer[i++] = c;   //guardo el caracter en el buffer
                putC(c);
            }
        }
    }
    return;
}


int shellInit() {
    while(1){
        startingLine();
        bufferControl();
    }
    return 0;
}