//
// Created by nicol on 5/22/2024.
//

#ifndef TPE_SYSCALLS_H
#define TPE_SYSCALLS_H
#define STDIN 0
#define STDOUT 1
#define ERROUT 2

void sys_write(int descriptor, char * str, int len);

void sys_read(int descriptor, int len, char * save);

void printCurrentHour();

#endif //TPE_SYSCALLS_H
