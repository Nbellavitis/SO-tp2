#include "include/usrSysCall.h"
#include "include/lib.h"
#include "include/Buffer.h"
#include "include/eliminator.h"

int state,flag;
char speed;
static char buffer[BUFFER] = {0};
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
void buffRead(){
    int i = 0;
    while (1) {
        char c = getC();
        if(c!=0){
            if (c == '\n' && state == TITLE){
                call_clear();
                state++;
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

void title(){
    call_setFontSize(2);
    call_moveCursorX((width/2)-(strlen("ELIMINATOR")/2) *8 * 2);
    call_moveCursorY(height/3);
    print(RED,"ELIMINATOR\n");
    call_moveCursorX((width/2)-(strlen("PRESS [ENTER] TO CONTINUE")/2) *8 * 2);
    print(RED,"PRESS [ENTER] TO CONTINUE\n");
    call_moveCursorX((width/2)-(strlen("PRESS [X] TO EXIT")/2) *8 * 2);
    print(RED,"PRESS [X] TO EXIT\n");
}
void configuration(){
    call_moveCursorX((width/2)-(strlen("CONFIGURATION")/2) *8 * 2);
    print(RED,"CONFIGURATION\n");
    call_moveCursorX((width/2)-(strlen("PRESS [ENTER] TO START")/2) *8 * 2);
    print(RED,"PRESS [ENTER] TO START\n");
    while(state == CONFIGURATION){
        print(RED,"GAME SPEED (1-4): ");
        gameSpeed();
        if(state == CONFIGURATION){
            print(RED,"INVALID SPEED\n");
        }
    }
    game();
}

void gameSpeed(){
    int i = 0;
    while (1) {
        char c = getC();
        if(c != 0){
            if (c == '\n'){
                putC(c,RED);
                if (i == 0){
                    clearBuffer();
                    return;
                }
                if((buffer[0] == '1' || buffer[0] == '2' || buffer[0] == '3' || buffer[0] == '4') && i == 1){
                    speed = buffer[0];
                    state++;
                }
                buffer[i]=0;
                clearBuffer();
                return;
            }else if (c == '\b'){
                if (i > 0){
                    i--;
                    putC(c,RED);
                }
            }else{
                if (i < RED){
                    buffer[i++] = c;
                    putC(c,RED);
                }
            }
        }
    }
    return;
}

void game(){
    int i,j,k,l;
    i=j=k=l=0;
    call_clear();
    call_drawRectangle(RED,0,0,height,width);
    call_drawRectangle(BLACK, 10, 10, height - 20, width - 20);
    call_drawRectangle(RED,width/2,height/2,10,10);
    while(i<500){
    call_sleepms(100);
    call_drawRectangle(RED,width/2 + i,height/2 + j,10,10);
     i+=10;
     j+=10;  
    }
}