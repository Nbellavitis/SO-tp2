#include "include/keyboardDriver.h"
#include "../include/keyboardBuffer.h"
#include <stdio.h>
#include "include/scanCode.h"
#include "include/videoDriver.h"
    static char keyMapRow = 0;
void keyboard_handler()
{

	uint16_t code = getKey();
	if(code < 0x80){    // Key pressed
    char * buff = getBufferAddress();  
   int buff_pos = getBufferPosition();
    if(code == LEFT_SHIFT || code == RIGHT_SHIFT){
      keyMapRow=1;
    }
    else if(keyMap[keyMapRow][code]!=0){
        if(keyMap[keyMapRow][code]!='\b'){
        buff[buff_pos++]=keyMap[keyMapRow][code];
        incBufferLen(1);
        setPos(buff_pos);
        }else{
            setPos(--buff_pos);
            incBufferLen(-1);
        }
    }
	} else {               // Key released
    code-=0x80;
    if(code == LEFT_SHIFT || code == RIGHT_SHIFT){  
      keyMapRow&=0xFE;
    }
	}

  return;
}


// void keyboard_handler(){
//     uint16_t key=getKey();
//     if(key > 0x80)
//         return;
//    int shift= (key < 0x80)
//     char * buff = getBufferAddress();  
//     int buff_pos = getBufferPosition();
//     if (buff_pos+1 < BUFF_SIZE){
//         setPos(buff_pos+1); 
//         buff[buff_pos+1] = 0;  
//     }else {
//         return;
//     }
//     buff[buff_pos]=scancodeShiftToAscii[key];
//     return;
// }