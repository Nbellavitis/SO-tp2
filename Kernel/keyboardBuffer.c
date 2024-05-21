#include <keyboardBuffer.h>
static struct keyboard_buffer buff = { 0, 0, {0} };

static buff_ptr ptr = &buff;


int getBufferPosition(){
return ptr->bufferPos;
}
void setPos(int newPosition){
    ptr->bufferPos=newPosition;
    return newPosition;             
}
char * getBufferAddress(){
    return ptr->buffer;
}
char getCharAt(int position){
    return ptr->buffer[position];
}
// void consumeBuffAt(int pos){       para que pingo la quiero
//     ptr->buff[pos] = 0;       
// }
void cleanBuffer(){
    char * bufferAux=ptr->buffer; 
    for(int i=0;i<ptr->bufferLen;i++){
        bufferAux[i]='\0';
    }
}
int getBufferLen(){
    return ptr->bufferLen;
}
