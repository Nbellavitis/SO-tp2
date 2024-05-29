#ifndef eliminator_h
#define eliminator_h
#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024
#define BUFFER 10 //pequeno para chequear
#define TITLE 0
#define CONFIGURATION 1
#define GAME 2
#define MIDGAME 3
#define RED 0x00FF0000
#define BLACK 0x00000000
#define GREEN 0x3342DD12

void startEliminator();
void buffRead();
void title();
void configuration();
void game();
void gameSpeed();

#endif