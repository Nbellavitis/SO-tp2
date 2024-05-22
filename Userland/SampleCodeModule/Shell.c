#include <Shell.h>
#include <buffer.h>

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
        char c = getChar();

        if (c == '\n'){
            putChar(c);
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
            putChar(c);

        }else{
            if (i != BUFFER_SIZE-1){
                buffer[i++] = c;   //guardo el caracter en el buffer
                putChar(c);
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