#include "include/usrSysCall.h"
#include "include/lib.h"
#define TITLE 0
static void buffRead();
static void configuration();
static void title();
int state,flag;
uint16_t width,height;
void startEliminator(){
    state=TITLE;
    height=call_getHeight();
    width=call_getWidth();
     flag=1;
    call_clear();
    title();
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
            }else if(c == 'x' && state == TITLE){
                call_clear();
                call_setFontSize(1);
                flag=0;
                return;
            }
    }
    return;
}
}

static void title(){
    call_setFontSize(2);
    call_moveCursorX((width/2)-(strlen("ELIMINATOR")/2) *8 * 2);
    call_moveCursorY(height/3);
    print(0x00FF0000,"ELIMINATOR\n");
    call_moveCursorX((width/2)-(strlen("PRESS [ENTER] TO CONTINUE")/2) *8 * 2);
    print(0x00FF0000,"PRESS [ENTER] TO CONTINUE\n");
    call_moveCursorX((width/2)-(strlen("PRESS [X] TO EXIT")/2) *8 * 2);
    print(0x00FF0000,"PRESS [X] TO EXIT\n");
}
static void configuration(){
call_moveCursorX((width/2)-(strlen("CONFIGURATION")/2) *8 * 2);
print(0x00FF0000,"CONFIGURATION\n");
}