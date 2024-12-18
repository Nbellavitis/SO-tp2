//
// Created by nicol on 5/22/2024.
//

#ifndef TPE_SYSCALLS_H
#define TPE_SYSCALLS_H
#define STDIN 0
#include "lib.h"
#define STDOUT 1
#define ERROUT 2

// writes in the file descriptor received the string, of length len in a color of HexColor
void sysWrite(int descriptor, const char * str, int len, uint32_t hexColor);

//reads from the character buffer until reaching the amount of characters len or emptying the buffer and saves the chars at save
int sysRead(int descriptor, char * save,int len);


#endif //TPE_SYSCALLS_H
