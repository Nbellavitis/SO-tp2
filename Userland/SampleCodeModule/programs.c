// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "include/lib.h"
#include "include/program.h"
#define TIME 1000
#include "include/usrSysCall.h"
#define EOF -1
#define MAX_BUFFER 2048
void prioTest(int argc, char *argv[]) { testPrio(); }

void processTest(int argc, char *argv[]) {

  if (argc != 2) {

    print(0xFFFFFFFF, "Usage: proctest <number_of_processes>\n");
    return;
  }

  if (satoi(argv[1]) <= 0) {

    print(0xFFFFFFFF, "debe ser mayor a 0\n");
    return;
  }

  return;
}
void runTestSync(int argc, char *argv[]) {

  if (argc != 2) {
    print(0xFFFFFFFF, "Usage: testSync <number_of_processes>\n");
    return;
  }
  testSync(argc, argv);
  return;
}

void cat() {
  char c;
  getC(&c);
  char comm[MAX_BUFFER] = {0};
  int i = 0;
  while (c != EOF) {
    if (c != 0) {
      putC(c, WHITE);
      comm[i++] = c;
      if (c == '\n') {
        comm[i] = '\0';
        print(WHITE, "%s", comm);
        comm[0] = '\0';
        i = 0;
      }
    }
    getC(&c);
  }
  putC('\n', WHITE);
  return;
}

void wc() {
  int lines = 1;
  char c;
  getC(&c);
  while (c != EOF) {
    if (c != 0) {
      if (c == '\n') {
        lines++;
      }
      putC(c, WHITE);
    }
    getC(&c);
  }
  putC('\n', WHITE);

  print(WHITE, "Line count: %d\n", --lines);
  putC('\n', WHITE);
}

void filter() {
  char c;
  getC(&c);
  char comm[MAX_BUFFER] = {0};
  int i = 0;
  while (c != EOF) {
    if (c != 0) {
      putC(c, WHITE);
      if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
          c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
        comm[i++] = c;
      }
    }
    getC(&c);
  }
  print(WHITE, "%s", comm);
  putC('\n', WHITE);
  return;
}

void loop() {
  while (1) {
    print(WHITE, "Hola soy pid: %d\n", getMyPid());
    callSleepMs(TIME);
  }
}