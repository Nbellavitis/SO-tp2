// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "include/Shell.h"
#include "include/Buffer.h"
#include "include/lib.h"
#include "include/usrSysCall.h"
#include "include/eliminator.h"
#include "include/program.h"
#include <stdio.h>
static char buffer[BUFFER_SIZE] = {0};
int exitFlag =0;
int registerFlag = 0;
typedef void (*CommandFunction)(void);
void lineRead(char * buffer);
void call_InvalidOp();
char reSize(char * buffer);
void call_div0();

void startingLine(){
    char * startingLine = "$>";
    putString(startingLine,GREEN);
    clearBuffer(buffer);
}

void showTime();
void resizeFont();
void getRegisters();
void clearTerminal();
void exitShell();
void testDiv0();
void testInvalidOp();
void testMemory();
void showMemStatus();
void testPriority();
void runProcessTest();
void showProcesses();
void killProcessCommand();
void runTestSync();
void runTestNoSync();
typedef struct {
     char *command;
    CommandFunction function;
     char *description;
} Command;

Command commands[] = {
    {"eliminator", startEliminator, "Game similar to tron(the movie)."},
    {"time", showTime, "Shows the actual time."},
    {"setFont", resizeFont, "Change the font size, receive an int from 1 to 2."},
    {"getRegisters", getRegisters, "Show the actual state of the registers."},
    {"clear", clearTerminal, "Empty the terminal."},
    {"exit", exitShell, "Kills the terminal."},
    {"div0", testDiv0, "Test the exception of the zero division."},
    {"invalidOp", testInvalidOp, "Test the exception of an invalid operand."},
    {"testmm", testMemory, "Allocates memory and runs the test."},
    {"mmStatus", showMemStatus, "Shows the memory status."},
    {"testPrio", testPriority, "Run the priority test."},
    {"testProcess", runProcessTest, "Run the process test."},
    {"ps", showProcesses, "Shows the process list."},
    {"kill", killProcessCommand, "Kills a process, use: kill n to kill the process with pid n."},
    {"testSync", runTestSync, "Run the sync test."},
    {"testNoSync", runTestNoSync, "Run the sync test without semaphores."}
};

void bufferControl(){
    int i = 0;
    while (1) {
        char c;
         getC(&c);

        if(c!=0 && c != '\t'){
        if (c == '\n'){
            putC(c,WHITE);
            if (i == 0){
                clearBuffer(buffer);
                return;
            }
            buffer[i]=0;
            lineRead(buffer);
            clearBuffer(buffer);
            return;

        }else if (c == '\b'){
            if (i > 0){
                i--;
                putC(c,WHITE);
            }

        }else{
            if (i < BUFFER_SIZE){
                buffer[i++] = c;
                putC(c,WHITE);
            }
        }
    }

    }
}


void executeCommand(const char *buffer) {
    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strncmp(buffer, commands[i].command, strlen(commands[i].command)) == 0) {
            commands[i].function();
            return;
        }
    }
    putString(buffer, WHITE);
    putString(": command not found\n", WHITE);
}


void showTime() {
    char time[9];
    call_timeClock(time);
    putString(time, WHITE);
    putString("\n", WHITE);
}

void resizeFont() {
    if (reSize(buffer) != 0) {
        call_clear();
    } else {
        putString("Enter a valid size (1 or 2) \n", RED);
    }
    clearBuffer(buffer);
}

void getRegisters() {
    call_printRegisters(1);
}

void clearTerminal() {
    call_clear();
    clearBuffer(buffer);
}

void exitShell() {
    exitFlag = 1;
    call_clear();
    clearBuffer(buffer);
}

void testDiv0() {
    call_div0();
}

void testInvalidOp() {
    call_InvalidOp();
}

void testMemory() {
    testmm();
}

void showMemStatus() {
    mmStatus();
}

void testPriority() {
    uint64_t rip = (uint64_t) test_prio;
 waitpid(createProcess(rip, 0, 1, 0, NULL));
}

void runProcessTest() {
    char **argv = allocMemory(2 * sizeof(char *));
    argv[1] = "1";
    argv[0] = "processtest";
    createProcess((uint64_t) processtest, 0, 0, 2, argv);
}

void showProcesses() {
    printAllProcesses(ps());
}

void killProcessCommand() {
    char *init = buffer + strlen("kill ");
    if (!strlen(init))
        return;
    killProcess(strToInt(init));
}
void runTestSync(){
  char **argv = allocMemory(2 * sizeof(char *));

  argv[1] = "1";
  argv[0] = "1000000";
 waitpid(createProcess((uint64_t) testSync, 0, 1, 2, argv));
}
void runTestNoSync(){
  char **argv = allocMemory(2 * sizeof(char *));
  argv[1] = "0";
  argv[0] = "10";
 waitpid(createProcess((uint64_t) testSync, 1, 1, 2, argv));
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
    } else {
        executeCommand(buffer);
    }
}
int shellInit() {
    char * start = "Welcome to Jungle OS, type help to get a list of commands.\n";
    putString(start,GREEN);
    clearBuffer(buffer);
    while(!exitFlag){
        startingLine();
        bufferControl();
    }
    return 0;
}
char reSize(char * buffer){
    char * init = buffer + strlen("setFont ");
    if(! strlen(init))
        return 0;
    return (char) call_setFontSize(strToInt(init));
}
void call_InvalidOp(){
    InvalidOpasm();
}
void call_div0(){
    int a=7;
    int b=0;
    a=a/b;
}