#include "include/Buffer.h"

void clearBuffer(char * buffer){
    for(int i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = 0;
    }
}