// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../Kernel/mm/mm.h"
#include "test_util.h"
#include <stdio.h>
#define ARENA_SIZE 40894464
extern  uint64_t test_mm(uint64_t argc, char *argv[]) ;
int main(){

    static uint32_t mem[ARENA_SIZE];
    if  ( mmInit((void *)mem, ARENA_SIZE) == -1){
        printf("ERROR:mem size has to be lower \n");
        return -1;
    }
    char *argv[] = {"266240"};
    test_mm(1, argv);
    return 0;

}