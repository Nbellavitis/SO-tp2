//
// Created by nicol on 5/22/2024.
//

#ifndef TPE_LIB_H
#define TPE_LIB_H
#include <stdint.h>
int getC(char * c);
void putC(char c,uint32_t hexColor);
void putInt(int num,uint32_t hexColor);
void print(uint32_t hexColor,const char * format, ...);
int strcmp(char * str1, char * str2);
int strcmpSpace(char * str1, char * str2);
int strlen(const char *str);
void putString(const char * s,uint32_t hexColor);
int strToInt(char * s);
int getBuffer(char * buf);
void strToUpper(char *str);
#endif //TPE_LIB_H
