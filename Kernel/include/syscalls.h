//
// Created by nicol on 5/22/2024.
//

#ifndef TPE_SYSCALLS_H
#define TPE_SYSCALLS_H
#define STDIN 0
#define STDOUT 1
#define ERROUT 2

void sys_write(int descriptor, char * str, int len);

void sys_read(int descriptor, char * save,int len);

void clock(char * buffer);


#endif //TPE_SYSCALLS_H
