#include "include/usrSysCall.h"
#include "include/lib.h"
#define TITLE 0
static void buffRead();
static void configuration();
int state;
void startEliminator(){
    state=TITLE;
    int flag=1;
    call_clear();
    call_printTitle();
    while(flag){
    buffRead();
    }
}
static void buffRead(){
    int i = 0;
    while (1) {
        char c = getC();
        if(c!=0){
            if (c == '\n' && state == TITLE){
                call_clear();
                configuration();
            }
    }
    return;
}
}

static void configuration(){
print("CONFIGURATION\n");
}