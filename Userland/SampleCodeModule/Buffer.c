// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "include/Buffer.h"
#include "include/lib.h"
#include "include/usrSysCall.h"

void clearBuffer(char *buffer) {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = 0;
  }
}
