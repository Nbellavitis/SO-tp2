
#ifndef SO_TP2_PIPE_H
#define SO_TP2_PIPE_H
#include "keyboardBuffer.h"
#include "lib.h"
#include "sems.h"
#define MAX_PIPES 16

void initPipes();
int pipeOpen(char * name);
int pipeWrite(char * name,const  char * str, int len);
int pipeRead(char * name, char * save, int len);
int pipeClose(char * name);
int pipeOpenAnon(char * name);
#endif //SO_TP2_PIPE_H
