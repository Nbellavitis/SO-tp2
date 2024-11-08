// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "include/Shell.h"
#include "include/Buffer.h"
#include "include/eliminator.h"
#include "include/lib.h"
#include "include/philosophers.h"
#include "include/program.h"
#include "include/usrSysCall.h"
#include <stdio.h>
static char buffer[BUFFER_SIZE] = {0};
int exitFlag = 0;
int registerFlag = 0;
typedef void (*CommandFunction)(int argc, char *argv[]);
void lineRead(char *buffer);

char reSize(char *buffer);

void startingLine() {
  char *startingLine = "$>";
  putString(startingLine, GREEN);
  clearBuffer(buffer);
}

void callDiv0();
void callInvalidOp();
void showTime(int argc, char *argv[]);
void resizeFont(int argc, char *argv[]);
void getRegisters(int argc, char *argv[]);
void clearTerminal(int argc, char *argv[]);
void exitShell(int argc, char *argv[]);
void testDiv0(int argc, char *argv[]);
void testInvalidOp(int argc, char *argv[]);
void showMemStatus(int argc, char *argv[]);
void testPriority(int argc, char *argv[]);
void showProcesses(int argc, char *argv[]);
void killProcessCommand(int argc, char *argv[]);
void runTestSync(int argc, char *argv[]);
void runTestNoSync(int argc, char *argv[]);
void testeandoo(int argc, char *argv[]);
void blockWrapper(int argc, char *argv[]);
void unblockWrapper(int argc, char *argv[]);
void niceWrapper(int argc, char *argv[]);
typedef struct {
  char *command;
  CommandFunction function;
  char *description;
  char *argv[5];
  int argc;
} Command;

Command commands[] = {
    {"eliminator",
     startEliminator,
     "Game similar to tron(the movie).",
     {"eliminator", NULL},
     1},
    {"time", showTime, "Shows the actual time.", {"time", NULL}, 1},
    {"setFont",
     resizeFont,
     "Change the font size, receive an int from 1 to 2.",
     {"setFont", NULL},
     1},
    {"getRegisters",
     getRegisters,
     "Show the actual state of the registers.",
     {"getRegisters", NULL},
     1},
    {"clear", clearTerminal, "Empty the terminal.", {"clear", NULL}, 1},
    {"exit", exitShell, "Kills the terminal.", {"exit", NULL}, 1},
    {"div0",
     testDiv0,
     "Test the exception of the zero division.",
     {"div0", NULL},
     1},
    {"invalidOp",
     testInvalidOp,
     "Test the exception of an invalid operand.",
     {"invalidOp", NULL},
     1},
    {"testmm",
     testMm,
     "Allocates memory and runs the test.",
     {"testmm", "266240", NULL},
     2},
    {"mmStatus",
     showMemStatus,
     "Shows the memory status.",
     {"mmStatus", NULL},
     1},
    {"testPrio", testPrio, "Run the priority test.", {"testPrio", NULL}, 1},
    {"testProcess",
     processTest,
     "Run the process test.",
     {"processtest", "1", NULL},
     2},
    {"ps", showProcesses, "Shows the process list.", {"ps", NULL}, 1},
    {"kill",
     killProcessCommand,
     "Kills a process, use: kill n to kill the process with pid n.",
     {"kill", NULL},
     1},
    {"block",
     blockWrapper,
     "block a process,use: block n to block the process with pid n.",
     {"block", NULL},
     1},
    {"unblock",
     unblockWrapper,
     "unblock a process,use: unblock n to unblock the process with pid n.",
     {"unblock", NULL},
     1},
    {"nice",
     niceWrapper,
     "nice a process,use: nice n m to set the priority of the process with pid "
     "n to m.",
     {"nice", NULL},
     1},
    {"cat", cat, "Reads from stdin and writes to stdout", {"cat", NULL}, 1},
    {"loop", loop, "Prints its pid every 100000 ms", {"loop", NULL}, 1},
    {"wc", wc, "Counts the number of lines in the input", {"wc", NULL}, 1},
    {"phylo",
     philo,
     "Runs the philosopher's problem simulation",
     {"phylo", "10", NULL},
     2},
    {"filter", filter, "Filters the input vowels", {"filter", NULL}, 1},
};

void bufferControl() {
  int i = 0;
  while (1) {
    char c;
    getC(&c);

    if (c != 0 && c != '\t' && c != EOF) {
      if (c == '\n') {
        putC(c, WHITE);
        if (i == 0) {
          clearBuffer(buffer);
          return;
        }
        buffer[i] = 0;
        lineRead(buffer);
        clearBuffer(buffer);
        return;

      } else if (c == '\b') {
        if (i > 0) {
          i--;
          putC(c, WHITE);
        }

      } else {
        if (i < BUFFER_SIZE) {
          buffer[i++] = c;
          putC(c, WHITE);
        }
      }
    }
  }
}

char *trimWhitespace(char *str) {
  char *end;
  if (str == NULL)
    return str;
  while (((unsigned char)*str) == ' ')
    str++;

  if (*str == 0)
    return str;

  end = str + strlen(str) - 1;
  while (end > str && (((unsigned char)*end) == ' '))
    end--;

  *(end + 1) = 0;

  return str;
}

void executePipedCommands(char *buffer) {
  int ground = !(buffer[strlen(buffer) - 1] == '&');
  if (!ground) {
    buffer[strlen(buffer) - 1] = '\0';
  }
  char *command1 = strtok(buffer, "|");
  char *command2 = strtok(NULL, "|");
  command1 = trimWhitespace(command1);
  command2 = trimWhitespace(command2);
  char *descriptors1[2] = {"tty", "shellPipe"};
  char *descriptors2[2] = {"shellPipe", "tty"};
  pipeOpenAnon("shellPipe");
  int pid1 = -1;
  Command cm1;
  Command cm2;
  int flag = 0;
  for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (strncmp(command1, commands[i].command, strlen(commands[i].command)) ==
        0) {
      flag++;
      cm1 = commands[i];
      break;
    }
  }
  if (flag == 0) {
    putString("Command not found\n", WHITE);
    return;
  }
  for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (strncmp(command2, commands[i].command, strlen(commands[i].command)) ==
        0) {
      flag++;
      cm2 = commands[i];
      break;
    }
  }
  if (flag == 1) {
    putString("Command not found\n", WHITE);
    return;
  }
  pid1 = createProcess((uint64_t)cm1.function, ground, cm1.argc, cm1.argv,
                       descriptors1);
  createProcess((uint64_t)cm2.function, 0, cm2.argc, cm2.argv, descriptors2);
  waitpid(pid1);
}

void executeCommand(const char *buffer) {
  int background = buffer[strlen(buffer) - 1] == '&';
  char commandBuffer[BUFFER_SIZE];
  strcpy(commandBuffer, buffer);

  if (background) {
    commandBuffer[strlen(commandBuffer) - 1] = '\0';
  }

  for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {

    if (strncmp(commandBuffer, commands[i].command,
                strlen(commands[i].command)) == 0) {

      char *descriptors[2] = {"tty", "tty"};
      waitpid(createProcess((uint64_t)commands[i].function, !background,
                            commands[i].argc, commands[i].argv, descriptors));
      return;
    }
  }

  putString(buffer, WHITE);
  putString(": command not found\n", WHITE);
}

void testeandoo(int argc, char *argv[]) { testeando(); }
void showTime(int argc, char *argv[]) {
  char time[9];
  callTimeClock(time);
  putString(time, WHITE);
  putString("\n", WHITE);
}

void resizeFont(int argc, char *argv[]) {
  if (reSize(buffer) != 0) {
    callClear();
  } else {
    putString("Enter a valid size (1 or 2) \n", RED);
  }
  clearBuffer(buffer);
}

void getRegisters(int argc, char *argv[]) { callPrintRegisters(1); }

void clearTerminal(int argc, char *argv[]) {
  callClear();
  clearBuffer(buffer);
}

void exitShell(int argc, char *argv[]) {
  exitFlag = 1;
  callClear();
  clearBuffer(buffer);
}

void testDiv0(int argc, char *argv[]) { callDiv0(); }
void blockWrapper(int argc, char *argv[]) {
  char *init = buffer + strlen("block ");
  if (!strlen(init))
    return;
  blockProcess(strToInt(init));
}
void unblockWrapper(int argc, char *argv[]) {
  char *init = buffer + strlen("unblock ");
  if (!strlen(init))
    return;
  unblockProcess(strToInt(init));
}
void niceWrapper(int argc, char *argv[]) {
  char *init = buffer + strlen("nice ");
  if (!strlen(init))
    return;
  char *pid = strtok(init, " ");
  char *prio = strtok(NULL, " ");
  changePrio(strToInt(pid), strToInt(prio));
}
void testInvalidOp(int argc, char *argv[]) { callInvalidOp(); }

void showMemStatus(int argc, char *argv[]) { mmStatus(); }

void testPriority(int argc, char *argv[]) { testPrio(); }

void showProcesses(int argc, char *argv[]) { printAllProcesses(ps()); }

void killProcessCommand(int argc, char *argv[]) {
  char *init = buffer + strlen("kill ");
  if (!strlen(init))
    return;
  killProcess(strToInt(init));
}

void lineRead(char *buffer) {
  if (strcmp(buffer, "help") == 0) {
    putString("The following commands may be used: \n", WHITE);
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
      putString(commands[i].command, WHITE);
      putString(": ", WHITE);
      putString(commands[i].description, WHITE);
      putString("\n", WHITE);
    }
  } else if (strcmp(buffer, "div0") == 0) {
    testDiv0(0, NULL);
  } else if (strcmp(buffer, "invalidOp") == 0) {
    testInvalidOp(0, NULL);
  } else {
    if (strstr(buffer, "|") != NULL) {
      executePipedCommands(buffer);
    } else {
      executeCommand(buffer);
    }
  }
}
int shellInit() {
  char *start = "Welcome to Jungle OS, type help to get a list of commands.\n";
  putString(start, GREEN);
  clearBuffer(buffer);
  while (!exitFlag) {
    startingLine();
    bufferControl();
  }
  return 0;
}
char reSize(char *buffer) {
  char *init = buffer + strlen("setFont ");
  if (!strlen(init))
    return 0;
  return (char)callSetFontSize(strToInt(init));
}
void callInvalidOp() { invalidOpAsm(); }
void callDiv0() {
  int a = 7;
  int b = 0;
  a = a / b;
}