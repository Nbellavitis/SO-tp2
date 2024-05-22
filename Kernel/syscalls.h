//
// Created by nicol on 5/22/2024.
//

#ifndef TPE_SYSCALLS_H
#define TPE_SYSCALLS_H
#define STDOUT 1
#define ERROUT 2

void sys_write(int descriptor, char * str, int len);

#endif //TPE_SYSCALLS_H
