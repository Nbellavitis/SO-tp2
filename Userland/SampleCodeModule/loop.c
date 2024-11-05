#include <syscalls.h>
#include "include/usrSysCall.h"
#include "include/lib.h"

#define TIME 100000


void loop(int argc, char *argv[]) {
    if (argc != 1) {
        print( WHITE, "Usage: loop\n");
        return;
    }

    while (1) {
        print( WHITE, "Hola soy pid: %d\n", getpid());
        call_sleepms(TIME);
    }
}