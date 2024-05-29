#include "include/usrSysCall.h"
#include "include/lib.h"
#include "include/Buffer.h"
#include "include/eliminator.h"
#define MOVE 8
void movePlayer1(int x,int y);
void initializePositions();
void midGame();
void fillWithZeros();
void movePlayer2(int x,int y);
void checkPrevKey1();
void checkPrevKey2();
int state,flag,posXplay1,posYplay1,posXplay2,posYplay2,points1,points2;
char speed;
char prevKey1='s';
char prevKey2='u';
static char buffer[BUFFER] = {0};
static uint16_t PositionMatrix[HEIGHT/MOVE][WIDTH/MOVE];

void buffReadTitle(){
    while(1) {
        char c;
        getC(&c);
        if (c == '\n') {
            call_clear();
            state++;
            configuration();
            return;
        } else if (c == 'x') {
            call_clear();
            call_setFontSize(1);
            flag = 0;
            return;
        }
    }
}
void buffReadMidGame(){
    while(1) {
        char c;
        getC(&c);
        if(c == 'y'){
            state=GAME;
            game();
            return;
        }else if(c == 'n'){
            call_clear();
            call_setFontSize(1);
            state=TITLE;
            flag = 0;
            return;
        }
    }


}
void startEliminator(){
    state=TITLE;
    points1=points2=0;
    flag=1;
    call_clear();
    title();
    while(flag){
        if(state == TITLE)
            buffReadTitle();
        if(state == MIDGAME)
            buffReadMidGame();
        if(state == GAME)
            buffRead();
    }
}

void clearBufferEliminator(){
    for(int i=0;i<BUFFER;i++){
        buffer[i]=0;
    }
}


void buffRead(){
    char c;
    while (getC(&c)) {

            if (state == GAME) {
                if (c == 'w' && prevKey1 != 's') {
                    prevKey1 = c;
                }
                else if (c == 's' && prevKey1 != 'w') {
                    prevKey1 = c;

                } else if (c == 'd' && prevKey1 != 'a') {
                    prevKey1 = c;

                } else if (c == 'a' && prevKey1 != 'd') {
                    prevKey1 = c;

                }
                if (c == 'u' && prevKey2 != 'j') {
                    prevKey2 = c;

                } else if (c == 'j' && prevKey2 != 'u') {
                    prevKey2 = c;

                } else if (c == 'k' && prevKey2 != 'h') {
                    prevKey2 = c;

                } else if (c == 'h' && prevKey2 != 'k') {
                    prevKey2 = c;
                }
            }
}
}

void title(){
    call_setFontSize(2);
    call_moveCursorX((WIDTH/2)-(strlen("ELIMINATOR")/2) *8 * 2);
    call_moveCursorY(HEIGHT/3);
    print(RED,"ELIMINATOR\n");
    call_moveCursorX((WIDTH/2)-(strlen("PRESS [ENTER] TO CONTINUE")/2) *8 * 2);
    print(RED,"PRESS [ENTER] TO CONTINUE\n");
    call_moveCursorX((WIDTH/2)-(strlen("PRESS [X] TO EXIT")/2) *8 * 2);
    print(RED,"PRESS [X] TO EXIT\n");
}
void configuration(){
    call_moveCursorX((WIDTH/2)-(strlen("CONFIGURATION")/2) *8 * 2);
    print(RED,"CONFIGURATION\n");
	call_moveCursorX((WIDTH/2)-(strlen("PLAYER 1 (RED) MOVES WITH A-W-S-D")/2) *8 * 2);
	print(RED,"PLAYER 1 (RED) MOVES WITH A-W-S-D\n");
	call_moveCursorX((WIDTH/2)-(strlen("PLAYER 2 (GREEN) MOVES WITH H-U-J-K")/2) *8 * 2);
	print(RED,"PLAYER 2 (GREEN) MOVES WITH H-U-J-K\n");
    call_moveCursorX((WIDTH/2)-(strlen("PRESS [ENTER] TO START")/2) *8 * 2);
    print(RED,"PRESS [ENTER] TO START\n");
    while(state == CONFIGURATION){
        print(RED,"GAME SPEED (1-4): ");
        gameSpeed();
        if(state == CONFIGURATION){
            print(RED,"INVALID SPEED\n");
        }
    }
    game();
    return;
}

void gameSpeed(){
    int i = 0;
    char c;
    while (1) {
        getC(&c);
        if(c != 0){
            if (c == '\n'){
                putC(c,RED);
                if (i == 0){
                    clearBufferEliminator();
                    return;
                }
                if((buffer[0] == '1' || buffer[0] == '2' || buffer[0] == '3' || buffer[0] == '4') && i == 1){
                    speed = strToInt(buffer);
                    state++;
                }
                buffer[i]=0;
                clearBufferEliminator();
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
}

void game(){
    call_clear();
    call_drawRectangle(RED,0,0,HEIGHT,WIDTH);
    call_drawRectangle(BLACK, MOVE, MOVE, HEIGHT - (MOVE*2), WIDTH - (MOVE*2));
    fillWithZeros();
    initializePositions();
    print(0xFFFFFFFF,"%d",speed);
    state = GAME;
    char c;
    while(getC(&c));
    while(state == GAME){
        buffRead();
        checkPrevKey1();
        checkPrevKey2();
        call_sleepms(30/speed); //a mas velocidad mas rapido
        if(PositionMatrix[posYplay1/MOVE][posXplay1/MOVE]==1){
            points2++;
            state++;
            midGame();
            return;
        }else if(PositionMatrix[posYplay2/MOVE][posXplay2/MOVE]==1){
            points1++;
            state++;
            midGame();
            return;
        }
        call_drawRectangle(RED,posXplay1,posYplay1,MOVE,MOVE);
        call_drawRectangle(GREEN,posXplay2,posYplay2,MOVE,MOVE);
        PositionMatrix[posYplay2/MOVE][posXplay2/MOVE]=1;
        PositionMatrix[posYplay1/MOVE][posXplay1/MOVE]=1;
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
void fillWithZeros(){
    for(int i=0;i<HEIGHT/MOVE;i++){
        for(int j=0;j<WIDTH/MOVE;j++){
            if(i==0 || i==HEIGHT/MOVE-1 || j==0 || j==WIDTH/MOVE-1){
                PositionMatrix[i][j]=1;
            }else
                PositionMatrix[i][j]=0;
        }
    }
}
void initializePositions(){
	prevKey1='s';
	prevKey2='u';
    posXplay1=WIDTH/2;
    posXplay2=WIDTH/2;
    posYplay1=0;
    posYplay2=HEIGHT-MOVE;
}
void midGame(){
    call_beep();
    call_moveCursorX((WIDTH/2)-(strlen("Player 1:")/2) *8 * 2);
    call_moveCursorY(HEIGHT/4);
    print(RED,"Player 1: %d\n", points1);
    call_moveCursorX((WIDTH/2)-(strlen("Player 2:")/2) *8 * 2);
    print(GREEN,"Player 2: %d\n", points2);
    call_moveCursorX((WIDTH/2)-(strlen("Do you want to continue? [Y/N]")/2) *8 * 2);
    print(RED,"Do you want to continue? [Y/N]\n");

    return;
}
