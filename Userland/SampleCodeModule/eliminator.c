#include "include/usrSysCall.h"
#include "include/lib.h"
#include "include/Buffer.h"
#include "include/eliminator.h"
#define MOVE 10
void movePlayer1(int x,int y);
void checkPrevKey1();
void checkPrevKey2();
int state,flag,posXplay1,posYplay1,k,l,posXplay2,posYplay2;
char speed;
char prevKey1='s';
char prevKey2='u';
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
            } else if(state == GAME){
                if(c == 'w' && prevKey1 != 's'){
                prevKey1=c;
                movePlayer1(0,-MOVE);
                return;
            }else if(c == 's' && prevKey1 != 'w' ){
                prevKey1=c;
                movePlayer1(0,MOVE);
                return;
            }else if(c == 'd' &&  prevKey1 != 'a'){
                prevKey1=c;
                 movePlayer1(MOVE,0);
                return;
            } else if(c == 'a' &&  prevKey1 != 'd'){
                prevKey1=c;
                movePlayer1(-MOVE,0);
                return;
            }else  if(c == 'u' && prevKey2 != 'j'){
                prevKey2=c;
                movePlayer2(0,-MOVE);
                return;
            }else if(c == 'j' && prevKey2 != 'u' ){
                prevKey2=c;
                movePlayer2(0,MOVE);
                return;
            }else if(c == 'k' &&  prevKey2 != 'h'){
                prevKey2=c;
                 movePlayer2(MOVE,0);
                return;
            } else if(c == 'h' &&  prevKey2 != 'k'){
                prevKey2=c;
                movePlayer2(-MOVE,0);
                return;
            }
    }}
        if(state == GAME ){
            checkPrevKey1();
            checkPrevKey2();
           }
    return;

}}

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
            }
            else{
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
    call_clear();
    call_drawRectangle(RED,0,0,height,width);
    call_drawRectangle(BLACK, 10, 10, height - 20, width - 20);
    posXplay1=posXplay2=width/2;
    posYplay1=0;
    posYplay2=height-10;
    while(1){
    buffRead();
    call_sleepms(5);
    call_drawRectangle(RED,posXplay1,posYplay1,10,10);
    call_drawRectangle(GREEN,posXplay2,posYplay2,10,10);
    }
}

void movePlayer1(int x,int y){
    posYplay1+=y;
    posXplay1+=x;
}
void movePlayer2(int x,int y){
    posYplay2+=y;
    posXplay2+=x;
}
void checkPrevKey1(){
    if(prevKey1 =='w'){
                movePlayer1(0,-MOVE);
                return;
            }
            else if(prevKey1 =='s'){
               movePlayer1(0,MOVE);
               return;
            }
            else if(prevKey1 =='d'){
                 movePlayer1(MOVE,0);
                 return;
            }
            else if(prevKey1 =='a'){
                 movePlayer1(-MOVE,0);
                 return;
}}
void checkPrevKey2(){
      if(prevKey2 =='u'){
                movePlayer2(0,-MOVE);
                return;
            }
            else if(prevKey2 =='j'){
               movePlayer2(0,MOVE);
               return;
            }
            else if(prevKey2 =='k'){
                 movePlayer2(MOVE,0);
                 return;
            }
            else if(prevKey2 =='h'){
                 movePlayer2(-MOVE,0);
                 return;
            }
}