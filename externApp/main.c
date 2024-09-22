#include "../Kernel/mm/mm.h"
#include "test_util.h"
#include <stdio.h>
#define ARENA_SIZE 40894464
int main(){

    static uint64_t mem[ARENA_SIZE];
    if  ( mmInit((void *)mem, ARENA_SIZE) == -1){
        printf("ERROR:mem size has to be lower \n");
        return -1;
    }
    char *argv[] = {"266240"};
    test_mm(1, argv);
    return 0;

}