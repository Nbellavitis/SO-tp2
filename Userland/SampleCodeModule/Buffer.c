#include "include/Buffer.h"
#include "include/lib.h"

void clearBuffer(char * buffer){
    for(int i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = 0;
    }
}

// Define an array to store command strings
const char *commands[] = {
    "eliminator",
    "time",
    "check Exception",
    "incSize",
    "getRegisters",
    "clear"
};

void lineRead(char *buffer) {
    if (strcmp(buffer, "help") == 0) {
        putString("Las siguientes comandos pueden ser utilizadas: \n");
        
        // Loop through the commands array and print each command
        for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
            putString(commands[i]);
            putString("\n");
        }
    }
    if (strcmp(buffer, "eliminator") == 0) {
        // Do something for the "eliminator" command
        putString("playing eliminator");
        putString("\n");

    }
    if (strcmp(buffer, "time") == 0) {
        // Do something for the "time" command
        putString("checking time");
        putString("\n");
    }
    if (strcmp(buffer, "check Exception") == 0) {
        // Do something for the "check" command
        putString("checking exception");
        putString("\n");
    }
    if (strcmp(buffer, "incSize") == 0) {
        // Do something for the "incSize" command
        putString("incrementing size");
        putString("\n");
    }
    if (strcmp(buffer, "getRegisters") == 0) {
        // Do something for the "getRegisters" command
        putString("geting Registers");
        putString("\n");
    }
    if(strcmp(buffer,"clean")){
        
    }
    // Add more conditions for other commands if needed
}
