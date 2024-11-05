#include "../include/pipe.h"
#define EOF -1
typedef struct Pipe{
    char * name;
    char buffer[BUFF_SIZE];
    char * mutex;
    char * writeSem;
    char * readSem;
    int readIndex;
    int writeIndex;
}Pipe;

Pipe * pipes[MAX_PIPES];

void initPipes(){
    for(int i = 0; i < MAX_PIPES; i++){
        pipes[i] = NULL;
    }
}

int pipeOpen(char * name){
    for(int i = 0; i < MAX_PIPES; i++){
        if(pipes[i] != NULL && strcmp(pipes[i]->name, name) == 0){
            return 1;
        }
    }
    for(int i = 0; i < MAX_PIPES; i++){
        if(pipes[i] == NULL){
            Pipe * pipe = (Pipe *) allocMemory(sizeof(Pipe));
            if(pipe == NULL){
                return 0;
            }
            pipe->name = name;
//            pipe->mutex = name;
//            if(!semOpen(pipe->sem, 1)){
//               return 0;
//            }
            pipe->writeSem = strAppend(name, "-write");
            if(!semOpen(pipe->writeSem, BUFF_SIZE)){
                freeMemory(pipe->writeSem);
                freeMemory(pipe);
               return 0;
            }
            pipe->readSem = strAppend(name, "-read");
            if(!semOpen(pipe->readSem, 0)){
                freeMemory(pipe->writeSem);
                freeMemory(pipe->readSem);
                freeMemory(pipe);
               return 0;
            }
            pipe->readIndex = 0;
            pipe->writeIndex = 0;
            pipes[i] = pipe;
            return 1;
        }
    }
    return 0;
}

int pipeWrite(char * name, const char * str, int len){
    for(int i = 0; i < MAX_PIPES; i++){
        if(pipes[i] != NULL && strcmp(pipes[i]->name, name) == 0){
            //semWait(pipes[i]->mutex);
            for(int j = 0; j < len; j++){
              semWait(pipes[i]->writeSem);
                pipes[i]->buffer[pipes[i]->writeIndex] = str[j];
                pipes[i]->writeIndex = (pipes[i]->writeIndex + 1) % BUFF_SIZE;
                semPost(pipes[i]->readSem);
            }
           // semPost(pipes[i]->mutex);
            return 1;
        }
    }
    return -1;
}
int pipeRead(char * name, char * save, int len){
    for(int i = 0; i < MAX_PIPES; i++){
        if(pipes[i] != NULL && strcmp(pipes[i]->name, name) == 0){
            //semWait(pipes[i]->mutex);
            for(int j = 0; j < len; j++){
                semWait(pipes[i]->readSem);
                save[j] = pipes[i]->buffer[pipes[i]->readIndex];
                pipes[i]->readIndex = (pipes[i]->readIndex + 1) % BUFF_SIZE;
                semPost(pipes[i]->writeSem);
            }
            //semPost(pipes[i]->mutex);
            return 1;
        }
    }
    return -1;
}
int pipeClose(char * name){
    for(int i = 0; i < MAX_PIPES; i++){
        if(pipes[i] != NULL && strcmp(pipes[i]->name, name) == 0){
            pipeWrite(name, (char *)EOF, 2);
            semClose(pipes[i]->writeSem);
            semClose(pipes[i]->readSem);
            freeMemory(pipes[i]->writeSem);
            freeMemory(pipes[i]->readSem);
            freeMemory(pipes[i]);
            pipes[i] = NULL;
            return 1;
        }
    }
    return -1;
}