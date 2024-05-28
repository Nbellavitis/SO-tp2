//
// Created by nicol on 5/22/2024.
//

#ifndef TPE_LIB_H
#define TPE_LIB_H
#include <stdint.h>
char getC();
void putC(char c,uint32_t hexColor);
void putInt(int num,uint32_t hexColor);

int own_scanf(char * format, ...);
int readInt(int* d);
int readString(char *s);
int readHexInt(int* d);
void print(uint32_t hexColor,const char * format, ...);
int strcmp(char * str1, char * str2);
int strcmpSpace(char * str1, char * str2);
int null_or_space(char c);
int null_or_newline(char c);
int strlen(const char *str);
void putString(char * s,uint32_t hexColor);
int strToInt(char * s);
#endif //TPE_LIB_H
