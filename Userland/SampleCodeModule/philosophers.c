#include "include/usrSysCall.h"
#include "include/lib.h"
#include <stdio.h>

#define TIME 100000000
#define INITIAL 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define WHITE 0xFFFFFFFF
#define MAX_PHILOSOPHERS 10
#define SEM_NAME_SIZE 20
#define ID_SIZE 10
#define TRUE 1
#define FALSE 0

typedef char bool;
char * auxSem = "sem_";
char * lastEating = "sem_lastEating";
char * almostLastThinking = "sem_almostLastThinking";
int * status;
uint64_t philosopherCount = INITIAL;
char ** semaphores;
pid_t * philosophers;

typedef struct {
    pid_t pid;
    char * id;
   char ** args;
   char * semName;
   char ** descriptors;
} Philosopher;

int getLeftIndex(int id) {
    return (id + philosopherCount - 1) % philosopherCount;
}

int getRightIndex(int id) {
    return (id + 1) % philosopherCount;
}

bool isValidState(int state) {
    return state == THINKING || state == HUNGRY || state == EATING;
}

bool isValidId(int id) {
    return id >= 0 && id < philosopherCount;
}

void printState(void) {
    semWait(auxSem);
    uint64_t current_count = philosopherCount;
    for (int i = 0; i < current_count && i < MAX_PHILOSOPHERS; i++) {
        if (isValidState(status[i])) {
            print(WHITE, status[i] == EATING ? "E" : ".");
            print(WHITE, " ");
        }
    }
    print(WHITE, "\n");
    semPost(auxSem);
}

void take_fork(int id) {

    semWait(auxSem);
    if (!isValidId(id)) return;
    if (!isValidState(status[id])) {
        print(WHITE, "Invalid philosopher state\n");
        semPost(auxSem);
        return;
    }
    status[id] = HUNGRY;
    semPost(auxSem);

    if (id % 2 == 0) {
        semWait(semaphores[id]);
        semWait(semaphores[getRightIndex(id)]);
    } else {
        semWait(semaphores[getRightIndex(id)]);
        semWait(semaphores[id]);
    }
    semWait(auxSem);
    status[id] = EATING;
    if(id == philosopherCount - 1){
        semPost(auxSem);
        semWait(lastEating);
    } else{
        semPost(auxSem);
    }
}

void put_fork(int id) {
    if (!isValidId(id)) return;

    if (id % 2 == 0) {
        semPost(semaphores[getRightIndex(id)]);
        semPost(semaphores[id]);
    } else {
        semPost(semaphores[id]);
        semPost(semaphores[getRightIndex(id)]);
    }
    semWait(auxSem);
    status[id] = THINKING;
    if(id == philosopherCount - 1){
        semPost(lastEating);
    }
    semPost(auxSem);

}
void put_fork_for_remove(int id){

    if (id % 2 == 0) {
        semPost(semaphores[getRightIndex(id)]);
//        semPost(semaphores[id]);
    } else {
//        semPost(semaphores[id]);
        semPost(semaphores[getRightIndex(id)]);
    }
    semWait(auxSem);
    status[id] = THINKING;
    if(id == philosopherCount - 1){
        semPost(lastEating);
    }
    semPost(auxSem);
}

void removePhilosopher(int id) {

    semWait(auxSem);
    if (status[id] == EATING) {
        print(WHITE, "Philosopher to be removed is eating\n");
        semPost(auxSem);
        put_fork(id);
        semWait(auxSem);
    }

    killProcess(philosophers[id]);
    semPost(auxSem);
    semClose(semaphores[id]);
    philosopherCount--;
    semPost(auxSem);
}

void philosopher(int argc, char *argv[]) {
    if (argc < 2) {
        print(WHITE, "Error: Philosopher needs ID argument\n");
        return;
    }
    int id = satoi(argv[1]);
    if (!isValidId(id)) {
        print(WHITE, "Error: Invalid philosopher ID\n");
        return;
    }

    while (1) {
        semWait(auxSem);
        if (!isValidId(id)) {
            print(WHITE, "Invalid philosopher ID\n");
            semPost(auxSem);
            return;
        }
//        if(id == philosopherCount - 2){
//            semPost(auxSem);
//            semWait(almostLastThinking);
//            semPost(almostLastThinking);
//            semWait(auxSem);
//        }
        status[id] = THINKING;
        semPost(auxSem);

        for (volatile int i = 0; i < TIME; i++);

        take_fork(id);

        printState();
        for (volatile int i = 0; i < TIME; i++);

        put_fork(id);
    }
}

void generateIdString(const char* prefix, int id, char* output, int maxLen) {
    if (!prefix || !output || maxLen <= 0) return;

    int i = 0;
    while (prefix[i] != '\0' && i < maxLen - 12) {
        output[i] = prefix[i];
        i++;
    }

    char numStr[11];
    intToStr(id, numStr);

    int j = 0;
    while (numStr[j] != '\0' && i < maxLen - 1) {
        output[i++] = numStr[j++];
    }

    output[i] = '\0';
}

void createPhilosophers(int philosCount) {
    semWait(auxSem);
    for (int i = 0; i < philosCount ; i++) {
        char * id= allocMemory(ID_SIZE);
        intToStr(i, id);

        char * semName = allocMemory(SEM_NAME_SIZE);
        generateIdString("sem_", i, semName, SEM_NAME_SIZE);
        char ** arg = allocMemory(3 * sizeof(char *));
        arg[0] = "philosopher";
        arg[1] = id;
        arg[2] = NULL;
        char **descriptors = allocMemory(2 * sizeof(char *));
        descriptors[0] = "tty";
        descriptors[1] = "tty";
        if (semOpen(semName, 1) == 0) {
            print(WHITE, "Failed to create semaphore ");
            print(WHITE, "%d",i);
            print(WHITE, "\n");
            killProcess(philosophers[i]);
            semPost(auxSem);
            return;
        }
        semaphores[i] = semName;
        philosophers[i] = createProcess((uint64_t)philosopher, 0, 2, arg, descriptors);
        if (philosophers[i] < 0) {
            print(WHITE, "Failed to create philosopher ");
            print(WHITE, "%d", i);
            print(WHITE, "\n");
            semPost(auxSem);
            return;
        }



        status[i] = THINKING;
    }
    semPost(auxSem);
}

void killPhilosophers() {
    if (!philosophers || !semaphores) return;

    semWait(auxSem);
    for (int i = 0; i < philosopherCount; i++) {
        if (philosophers[i] > 0) {
            killProcess(philosophers[i]);
            philosophers[i] = -1;
        }
        if (semaphores[i] != NULL) {
            semClose(semaphores[i]);
            semaphores[i] = NULL;
        }
    }
    semPost(auxSem);
}

void philo(int argc, char *argv[]) {
    if (argc != 2) {
        print(WHITE, "Usage: philo <max_number>\n");
        return;
    }

    int maxPhilos = satoi(argv[1]);
    print (WHITE, "Max philosophers: %d\n", maxPhilos);
    if (maxPhilos <= 0 || maxPhilos > MAX_PHILOSOPHERS) {
        print(WHITE, "Invalid number of philosophers (must be between 1 and ");
        print(WHITE, "%d", MAX_PHILOSOPHERS);
        print(WHITE, ")\n");
        return;
    }

    semaphores = allocMemory(sizeof(char *) * maxPhilos);
    if (!semaphores) {
        print(WHITE, "Memory allocation failed\n");
        return;
    }
    philosophers = allocMemory(sizeof(pid_t) * maxPhilos);
    if (!philosophers) {
        print(WHITE, "Memory allocation failed\n");
        freeMemory(semaphores);
        return;
    }
    status = allocMemory(sizeof(int) * maxPhilos);
    if (!status) {
        print(WHITE, "Memory allocation failed\n");
        freeMemory(semaphores);
        freeMemory(philosophers);
        return;
    }

    if (semOpen(auxSem, 1) == 0) {
        print(WHITE, "Failed to create auxiliary semaphore\n");
        freeMemory(status);
        freeMemory(semaphores);
        freeMemory(philosophers);
        return;
    }
    if(semOpen(lastEating, 1) == 0){
        print(WHITE, "Failed to create lastEating semaphore\n");
        freeMemory(status);
        freeMemory(semaphores);
        freeMemory(philosophers);
        semClose(auxSem);
        return;
    }
    if(semOpen(almostLastThinking, 1) == 0){
        print(WHITE, "Failed to create almostLastThinking semaphore\n");
        freeMemory(status);
        freeMemory(semaphores);
        freeMemory(philosophers);
        semClose(auxSem);
        semClose(lastEating);
        return;
    }

    createPhilosophers(INITIAL);
    print(WHITE, "\nCommands: 'q' to quit, 'a' to add philosopher, 'r' to remove philosopher\n");

    char c = 0;
    while (getC(&c) >= 0 && c != 'q' && philosopherCount > 0 && c != EOF) {
        switch (c) {
            case 'q':
                killPhilosophers();
                semClose(auxSem);
                semClose(lastEating);
                freeMemory(semaphores);
                freeMemory(philosophers);
                freeMemory(status);
                return;
            case 'a':
                semWait(auxSem);
                if (philosopherCount < maxPhilos) {
                    char * id= allocMemory(ID_SIZE);
                    intToStr(philosopherCount, id);
                    char * semName= allocMemory(SEM_NAME_SIZE);
                    generateIdString("sem_", philosopherCount, semName, SEM_NAME_SIZE);
                    char ** arg = allocMemory(3 * sizeof(char *));
                    arg[0] = "philosopher";
                    arg[1] = id;
                    arg[2] = NULL;
                    char **descriptors = allocMemory(2 * sizeof(char *));
                    descriptors[0] = "tty";
                    descriptors[1] = "tty";
                    if (semOpen(semName, 1) == 0) {
                        print(WHITE, "Failed to create semaphore ");
                        print(WHITE, "%d", philosopherCount);
                        print(WHITE, "\n");
                        killProcess(philosophers[philosopherCount]);
                        semPost(auxSem);
                        return;
                    }
                    semaphores[philosopherCount] = semName;
                    semPost(auxSem);
                    semWait(lastEating);
                    pid_t newPhil = createProcess((uint64_t) philosopher, 0,  2, arg,descriptors);
                    if (newPhil < 0) {
                        print(WHITE, "Failed to create new philosopher\n");
                        semPost(auxSem);
                        break;

                    }
                    semPost(lastEating);
                    semWait(auxSem);

                    status[philosopherCount] = THINKING;
                    philosophers[philosopherCount] = newPhil;
                    philosopherCount ++;
                }
                semPost(auxSem);
                break;
            case 'r':
                semWait(auxSem);
                if (philosopherCount > INITIAL) {
                    removePhilosopher(philosopherCount - 1);
                }
                semPost(auxSem);
                break;
        }
    }
    killPhilosophers();
    semClose(auxSem);
    freeMemory(semaphores);
    freeMemory(philosophers);
    freeMemory(status);
    return;
}
