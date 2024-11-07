// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "include/Shell.h"
#include "include/Buffer.h"
#include "include/lib.h"
#include "include/usrSysCall.h"
#include "include/eliminator.h"
#include "include/program.h"
#include "include/philosophers.h"
#include <stdio.h>
static char buffer[BUFFER_SIZE] = {0};
int exitFlag =0;
int registerFlag = 0;
typedef void (*CommandFunction)(int argc, char *argv[]);
void lineRead(char * buffer);

char reSize(char * buffer);


void startingLine(){
    char * startingLine = "$>";
    putString(startingLine,GREEN);
    clearBuffer(buffer);
}


void call_div0();
void call_InvalidOp();
void showTime(int argc, char *argv[]);
void resizeFont(int argc, char *argv[]);
void getRegisters(int argc, char *argv[]);
void clearTerminal(int argc, char *argv[]);
void exitShell(int argc, char *argv[]);
void testDiv0(int argc, char *argv[]);
void testInvalidOp(int argc, char *argv[]);
void testMemory(int argc, char *argv[]);
void showMemStatus(int argc, char *argv[]);
void testPriority(int argc, char *argv[]);
void runProcessTest(int argc, char *argv[]);
void showProcesses(int argc, char *argv[]);
void killProcessCommand(int argc, char *argv[]);
void runTestSync(int argc, char *argv[]);
void runTestNoSync(int argc, char *argv[]);
void testeandoo(int argc, char *argv[]);
void runPhilo(int argc, char *argv[]);
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
    {"testNoSync", runTestNoSync, "Run the sync test without semaphores."},
    {"testeando", testeandoo, "testeando"},
    {"cat",cat,"Reads from stdin and writes to stdout"},
    {"loop",loop,"prints his pid every 100000 ms"},
    {"wc",wc,"Counts the number of lines in the input"},
    {"philo",runPhilo,"run philo"},
 	{"filter",filter,"filters the inputs vowels"},

};

void bufferControl(){
    int i = 0;
    while (1) {
        char c;
         getC(&c);

        if(c!=0 && c != '\t' && c!= EOF){
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

char *trimWhitespace(char *str) {
    char *end;


    while (((unsigned char)*str) == ' ') str++;

    if (*str == 0)
        return str;


    end = str + strlen(str) - 1;
    while (end > str && (((unsigned char)*end) == ' ')) end--;


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
    char **descriptors1 = allocMemory(2 * sizeof(char *));
    descriptors1[0] = "tty";
    descriptors1[1] = "shellPipe";
    char **descriptors2 = allocMemory(2 * sizeof(char *));
    descriptors2[0] = "shellPipe";
    descriptors2[1] = "tty";
    pipeOpenAnon("shellPipe");
   int pid1=-1;
   CommandFunction cm1 = NULL;
   CommandFunction cm2 = NULL;
	for(int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strncmp(command1, commands[i].command, strlen(commands[i].command)) == 0) {
			cm1=commands[i].function;
            break;
        }
    }
    if(cm1==NULL){
        freeMemory(descriptors1);
        freeMemory(descriptors2);
        putString("Command not found\n",WHITE);
        return;
    }
    for(int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
        if (strncmp(command2, commands[i].command, strlen(commands[i].command)) == 0) {
          cm2=commands[i].function;
            break;
        }
    }
	if(cm2==NULL){
        freeMemory(descriptors1);
        freeMemory(descriptors2);
        putString("Command not found\n",WHITE);
        return;
        }
    pid1=createProcess((uint64_t)cm1, ground, 2, descriptors1, descriptors1);
    createProcess((uint64_t)cm2, 0, 2, descriptors2, descriptors2);
    waitpid(pid1);
    if(ground == 1) {
        pipeClose("shellPipe");
        freeMemory(descriptors1);
        freeMemory(descriptors2);
    }
}

void executeCommand(const char *buffer) {
    int background = buffer[strlen(buffer) - 1] == '&';
    char commandBuffer[BUFFER_SIZE];
    strcpy(commandBuffer, buffer);

    if (background) {
        commandBuffer[strlen(commandBuffer) - 1] = '\0';
    }



    for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {

        if (strncmp(commandBuffer, commands[i].command, strlen(commands[i].command)) == 0) {
        char **descriptors = allocMemory(2 * sizeof(char *));
        descriptors[0] = "tty";
        descriptors[1] = "tty";
        waitpid(createProcess((uint64_t)commands[i].function, !background, 0, NULL, descriptors));
        return;
        }
    }

    putString(buffer, WHITE);
    putString(": command not found\n", WHITE);
}

void testeandoo(int argc, char *argv[]){
    testeando();
}
void showTime(int argc, char *argv[]) {
    char time[9];
    call_timeClock(time);
    putString(time, WHITE);
    putString("\n", WHITE);
}

void resizeFont(int argc, char *argv[]) {
    if (reSize(buffer) != 0) {
        call_clear();
    } else {
        putString("Enter a valid size (1 or 2) \n", RED);
    }
    clearBuffer(buffer);
}

void getRegisters(int argc, char *argv[]) {
    call_printRegisters(1);
}

void clearTerminal(int argc, char *argv[]) {
    call_clear();
    clearBuffer(buffer);
}

void exitShell(int argc, char *argv[]) {
    exitFlag = 1;
    call_clear();
    clearBuffer(buffer);
}

void testDiv0(int argc, char *argv[]) {
    call_div0();
}

void testInvalidOp(int argc, char *argv[]) {
    call_InvalidOp();
}

void testMemory(int argc, char *argv[]) {
    testmm();
}

void showMemStatus(int argc, char *argv[]) {
    mmStatus();
}

void testPriority(int argc, char *argv[]) {
 	test_prio();
}

void runProcessTest(int argc, char *argv[]) {
    char **argv2 = allocMemory(2 * sizeof(char *));
    argv2[1] = "1";
    argv2[0] = "processtest";
    processtest(2, argv);
}

void showProcesses(int argc, char *argv[]) {
    printAllProcesses(ps());
}

void killProcessCommand(int argc, char *argv[]) {
    char *init = buffer + strlen("kill ");
    if (!strlen(init))
        return;
    killProcess(strToInt(init));
}
void runTestSync(int argc, char *argv[]){
  char **argv2 = allocMemory(2 * sizeof(char *));
  argv2[1] = "1";
  argv2[0] = "50000";
  testSync(2,argv2);
}
void runTestNoSync(int argc, char *argv[]){
  char **argv2 = allocMemory(2 * sizeof(char *));
  argv2[1] = "0";
  argv2[0] = "10";
  testSync(2,argv2);
}

void runPhilo(int argc, char *argv[]){
    char **argv2 = allocMemory(2 * sizeof(char *));
    argv2[0] = "1";
    argv2[1] = "10";
    philo(2,argv2);
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
    }else if(strcmp(buffer,"div0")==0) {
        testDiv0(0, NULL);
    }else if (strcmp(buffer,"invalidOp")==0){
        testInvalidOp(0, NULL);
    } else {
      if(strstr(buffer,"|") != NULL){
		executePipedCommands(buffer);
        }else{
        executeCommand(buffer);
        }
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