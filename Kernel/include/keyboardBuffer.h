#ifndef keyboardBuffer_h
#define keyboardBuffer_h
#include <stdint.h>
#define BUFF_SIZE 4096
typedef struct keyboard_buffer* buff_ptr; 

struct keyboard_buffer { 
    int bufferPos;
    int bufferLen;
    char buffer[BUFF_SIZE];
};
int getBufferPosition();
void setPos(int newPosition); //unsigned??
char * getBufferAddress();
char getCharAt(int position);
//void consumeBufferAt(int position);
void cleanBuffer();
int getBufferLen();

#endif