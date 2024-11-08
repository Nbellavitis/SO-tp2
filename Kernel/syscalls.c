// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "include/syscalls.h"
#include "Drivers/include/videoDriver.h"
#include "include/keyboardBuffer.h"
#include "include/lib.h"
#include "include/libasm.h"
#include "include/pipe.h"
#include "include/process.h"
#include <naiveConsole.h>
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void sysWrite(int descriptor, const char *str, int len, uint32_t hexColor) {
  switch (descriptor) {
  case STDOUT:
    if (strcmp(getActiveProcess()->fd[STDOUT], "tty") == 0) {
      drawWordLen(hexColor, str, len);
      return;
    } else {
      pipeWrite(getActiveProcess()->fd[STDOUT], str, len);
      return;
    }
  case ERROUT:
    drawWordLen(0x00ff0000, str, len);
    return;
  default:
    drawWord(0x00ff0000, "no such descriptor");
    return;
  }
}

int sysRead(int descriptor, char *save, int len) {
  if (descriptor != STDIN) {
    drawWord(0x00ff0000, "no such descriptor");
  }
  if (strcmp(getActiveProcess()->fd[STDIN], "null") == 0) {
    return 0;
  }
  if (strcmp(getActiveProcess()->fd[STDIN], "tty") == 0) {

    int length = MIN(len, getBufferLen());
    int i = 0;
    do {
      semWait("STDIN");
      int n = getBufferPosition();
      save[i] = getCharAt(n);
      if (save[i] == 0) {
        return 0;
      }
      consumeBufferAt(n);
      i++;
    } while (i < length);

    return length;
  } else {
    return pipeRead(getActiveProcess()->fd[STDIN], save, len);
  }
}

void twoChars(char *first, int j, char *app) {
  for (int i = 0; i < 2; i++) {
    first[j + i] = app[i];
  }
}
