// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "../include/pipe.h"
#include "../Drivers/include/videoDriver.h"
#define EOF -1
typedef struct Pipe {
  char *name;
  char buffer[BUFF_SIZE];
  char *writeSem;
  char *readSem;
  int readIndex;
  int writeIndex;
  int attachedProcesses;
  int pidWrite;
  int pidRead;
} Pipe;
char eof = EOF;
Pipe *pipes[MAX_PIPES];
int pipeCount = 0;

void initPipes() {
  for (int i = 0; i < MAX_PIPES; i++) {
    pipes[i] = NULL;
  }
}

int pipeOpen(char *name) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i] != NULL && strcmp(pipes[i]->name, name) == 0) {
      if (pipes[i]->attachedProcesses == 2) {
        return 0;
      }
      pipes[i]->attachedProcesses++;
      return 1;
    }
  }
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i] == NULL) {
      Pipe *pipe = (Pipe *)allocMemory(sizeof(Pipe));
      if (pipe == NULL) {
        return 0;
      }
      pipe->name = name;
      pipe->writeSem = strAppend(name, "-write");
      if (!semOpen(pipe->writeSem, BUFF_SIZE)) {
        freeMemory(pipe->writeSem);
        freeMemory(pipe);
        return 0;
      }
      pipe->readSem = strAppend(name, "-read");
      if (!semOpen(pipe->readSem, 0)) {
        freeMemory(pipe->writeSem);
        freeMemory(pipe->readSem);
        freeMemory(pipe);
        return 0;
      }
      pipe->readIndex = 0;
      pipe->writeIndex = 0;
      pipe->pidRead = -1;
      pipe->pidWrite = -1;
      pipes[i] = pipe;
      pipe->attachedProcesses = 1;
      pipeCount++;
      return 1;
    }
  }
  return 0;
}

int pipeWrite(char *name, const char *str, int len) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i] != NULL && strcmp(pipes[i]->name, name) == 0) {
      if (pipes[i]->pidWrite == -1) {
        pipes[i]->pidWrite = getActivePid();
      } else if (pipes[i]->pidWrite != getActivePid()) {
        return -1;
      }
      for (int j = 0; j < len; j++) {
        semWait(pipes[i]->writeSem);
        pipes[i]->buffer[pipes[i]->writeIndex] = str[j];
        pipes[i]->writeIndex = (pipes[i]->writeIndex + 1) % BUFF_SIZE;
        semPost(pipes[i]->readSem);
      }
      return 1;
    }
  }
  return -1;
}
int pipeRead(char *name, char *save, int len) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i] != NULL && strcmp(pipes[i]->name, name) == 0) {
      if (pipes[i]->pidRead == -1) {
        pipes[i]->pidRead = getActivePid();
      } else if (pipes[i]->pidRead != getActivePid()) {
        return -1;
      }
      for (int j = 0; j < len; j++) {
        semWait(pipes[i]->readSem);
        save[j] = pipes[i]->buffer[pipes[i]->readIndex];
        pipes[i]->readIndex = (pipes[i]->readIndex + 1) % BUFF_SIZE;
        semPost(pipes[i]->writeSem);
      }
      return 1;
    }
  }
  return -1;
}
int pipeClose(char *name) {
  for (int i = 0; i < MAX_PIPES; i++) {
    if (pipes[i] != NULL && strcmp(pipes[i]->name, name) == 0) {
      pipeWrite(name, &eof, 1);
      if (--pipes[i]->attachedProcesses != 0) {
        return 0;
      }
      semClose(pipes[i]->writeSem);
      semClose(pipes[i]->readSem);
      freeMemory(pipes[i]->writeSem);
      freeMemory(pipes[i]->readSem);
      freeMemory(pipes[i]);
      pipeCount--;
      pipes[i] = NULL;
      return 1;
    }
  }
  return -1;
}
int pipeOpenAnon(char *name) {
  pipeOpen(name);
  return pipeOpen(name);
}