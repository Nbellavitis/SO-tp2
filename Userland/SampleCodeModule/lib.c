#include "include/lib.h"
#include "include/usrSysCall.h"
#include <stdarg.h>

char getC(){
    char c;
    call_sys_read(STDIN,&c, 1);
    return c;
}
void putC(char c){
    call_sys_write( STDOUT,&c, 1);
}

void putInt(int num){ // MODIFICAR DPS
        if (num < 0) {
            putC('-');
            num = -num;
        }

        int divisor = 1;
        while (num / divisor >= 10) {
            divisor *= 10;
        }

        while (divisor > 0) {
            int digit = num / divisor;
            putC('0' + digit);
            num %= divisor;
            divisor /= 10;
        }
}
void putString(char * s){
    call_sys_write( STDOUT,s, strlen(s));
}


void print(const char * str, ...){
    va_list args;
    va_start(args, str);


    while(*str != '\0'){
        if(*str == '%'){
            str++;

            switch(*str){
                case 'c': {
                    char c = va_arg(args, char*);
                    putC(c);
                    break;
                }
                case 'd': {
                    int d = va_arg(args, int*);
                    putInt(d);
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    putString(s);
                    break;
                }
                case 'x':{
                    int * d = va_arg(args,int*);
                    char s[17];
                    toHexa(d,s);
                    putString(s);
                    break;
                }
            }
        } else {
            putC(*str);
        }
        str++;
    }

    va_end(args);
    return;
}

void toHexa(int * num, char * str){
    int i = 0;
    do {
        int b = *num % 16;
        str[i++] = (b < 10) ? ('0' + b) : ('A' + b - 10);
        *num /= 16;
    } while (*num != 0);
    str[i] = '\0';

    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }


}
int strlen(const char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
int strcmp( char *str1,  char *str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}
int strcmpSpace(char *str1, char *str2 ){
    while (*str1 && *str2 && *str1 == *str2 ) {
        str1++;
        str2++;
    }
    if(*str1 == ' ' || *str2 == ' '){
        return 0;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

int pow(int base, int pow){
    int toR= 1;
    for(int i=0; i< pow;i++){
        toR*= base;
    }
    return toR;
}
int strToInt(char * str){
    int n = strlen(str);
    int rta=0;
    for(int i=0; i<n; i++){
       rta += (*str - '0') * pow(10,n-i-1);
    }
    return rta;
}
