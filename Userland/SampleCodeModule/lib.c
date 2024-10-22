// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "include/lib.h"
#include "include/usrSysCall.h"
#include <stdarg.h>
#include <stddef.h>
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - ('a' - 'A')) : (c))


int getC(char * c){
    return call_sys_read(STDIN,c, 1);
}
int getBuffer(char * buf){
    return call_sys_read(STDIN,buf, 100);
}
void putC(char c,uint32_t hexColor){
    call_sys_write( STDOUT,&c, 1,hexColor);
}

void putInt(uint64_t num,uint32_t hexColor){
        if (num < 0) {
            putC('-',hexColor);
            num = -num;
        }

        uint64_t divisor = 1;
        while (num / divisor >= 10) {
            divisor *= 10;
        }

        while (divisor > 0) {

            int digit = num / divisor;
            putC('0' + digit,hexColor);
            num %= divisor;
            divisor /= 10;
        }
}
void putString(const char * s,uint32_t hexColor){
    call_sys_write( STDOUT,s, strlen(s),hexColor);
}


void print(uint32_t hexColor,const char * str, ...){
    va_list args;
    va_start(args, str);


    while(*str != '\0'){
        if(*str == '%'){
            str++;

            switch(*str){
                case 'c': {
                    char c = va_arg(args, int);
                    putC(c,hexColor);
                    break;
                }
                case 'd': {
                    int d = va_arg(args, int);
                    putInt(d,hexColor);
                    break;
                }
                case 's': {
                    char* s = va_arg(args, char*);
                    putString(s,hexColor);
                    break;
                }
            }
        } else {
            putC(*str,hexColor);
        }
        str++;
    }

    va_end(args);
    return;
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
int strncmp(const char *str1, const char *str2, size_t n) {
    while (n && *str1 && (*str1 == *str2)) {
        str1++;
        str2++;
        n--;
    }
    if (n == 0) {
        return 0;
    } else {
        return *(unsigned char *)str1 - *(unsigned char *)str2;
    }
}
char * cutString(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '\0';  // Reemplazar el espacio con el terminador nulo
            return str;
        }
    }
    return str;
}

void reverseStr(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
void intToStr(uint64_t num, char* str) {
    int i = 0;
    int isNegative = 0;


    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num != 0);


    if (isNegative) {
        str[i++] = '-';
    }


    str[i] = '\0';


    reverseStr(str, i);
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
       rta += (str[i] - '0') * pow(10,n-i-1);
    }
    return rta;
}


// Function to convert a string to uppercase
void strToUpper(char *str) {
    while (*str) {
        *str = TO_UPPER(*str);
        str++;
    }
}

void printProcess(processInfoPtr process){
    print(WHITE,"%d      ",process->pid);
    if(process->ppid == -1){
        print(WHITE, "-1        ");
    }else{
    print(WHITE, "%d        ", process->ppid);
    }
    print(WHITE,"%d    ",process->rsp);
    print(WHITE,"%d     ",process->stackBase);
    print(WHITE,"%d     ",process->rip);
    print(WHITE,"%d          ",process->ground);
    print(WHITE,"%d        ",process->priority);
    print(WHITE,"%d    ",process->status);
   // print(WHITE,"%s    ",process->name);
}

void printAllProcesses(processInfoPtr * processes){
    print(WHITE,"PID    PPID     RSP        StackBase    RIP       Ground   Priority   Status    Name\n");
    for(int i=0; processes[i] != NULL; i++){
        printProcess(processes[i]);
        freeMemory(processes[i]);
        putC('\n',WHITE);
    }
    freeMemory(processes);   
}

