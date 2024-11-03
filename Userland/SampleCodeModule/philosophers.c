#include <syscalls.h>
#include "include/usrSysCall.h"
#include "include/lib.h"

#define TIME 1000000
#define INITIAL 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define WHITE 0xFFFFFFFF
#define MAX_PHILOSOPHERS 10
#define SEM_NAME_SIZE 20
#define ID_SIZE 10

char * auxSem = "sem_";

int * status;
uint64_t philosopherCount = INITIAL;
char ** semaphores;
pid_t * philosophers;

int getLeftIndex(int id) {
    return (id + philosopherCount - 1) % philosopherCount;
}

int getRightIndex(int id) {
    return (id + 1) % philosopherCount;
}

void printState(void) {
    semWait(auxSem);
    for (int i = 0; i < philosopherCount; i++) {
        print(status[i] == EATING ? "E" : ".", WHITE);
        print(" ", WHITE);

    }
    print("\n", WHITE);
    semPost(auxSem);
}

void take_fork(int id) {
    semWait(auxSem);
    status[id] = HUNGRY;
    semPost(auxSem);
    if (id % 2 == 0) {
        semWait(semaphores[id]);
        semWait(semaphores[getRightIndex(id)]);
    } else {
        semWait(semaphores[getRightIndex(id)]);
        semWait(semaphores[id]);
    }
}

void put_fork(int id) {
    if (id % 2 == 0) {
        semPost(semaphores[getRightIndex(id)]);
        semPost(semaphores[id]);
    } else {
        semPost(semaphores[id]);
        semPost(semaphores[getRightIndex(id)]);
    }
    semWait(auxSem);
    status[id] = THINKING;
    semPost(auxSem);
}

void *philosopher(int argc, char *argv[]) {
    if (argc < 2) {
        print("Error: Philosopher needs ID argument\n", WHITE);
        return;
    }

    int id = strToInt(argv[1]);
    if (id < 0 || id >= philosopherCount) {
        print("Error: Invalid philosopher ID\n", WHITE);
        return;
    }

    while (1) {
        semWait(auxSem);
        status[id] = THINKING;
        semPost(auxSem);
        for (volatile int i = 0; i < TIME; i++);

        take_fork(id);
        semWait(auxSem);
        status[id] = EATING;
        semPost(auxSem);
        printState();
        for (volatile int i = 0; i < TIME; i++);

        put_fork(id);
    }
    return ;
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

void createPhilosophers(int maxPhilos) {
    for (int i = 0; i < maxPhilos; i++) {
        char id[ID_SIZE];
        intToStr(i, id);

        char semName[SEM_NAME_SIZE];
        generateIdString("sem_", i, semName, SEM_NAME_SIZE);

        char *arg[] = {"philosopher", id, NULL};
        philosophers[i] = createProcess((uint64_t)philosopher, 1, 1, 2, arg);
        if (philosophers[i] < 0) {
            print("Failed to create philosopher ", WHITE);
            print(i, WHITE);
            print("\n", WHITE);
            return;
        }

        semaphores[i] = semOpen(semName, 1);
        if (semaphores[i] == NULL) {
            print("Failed to create semaphore ", WHITE);
            print(i, WHITE);
            print("\n", WHITE);
            killProcess(philosophers[i]);
            return;
        }
    }
}

void killPhilosophers() {
    if (!philosophers || !semaphores) return;

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
}

void philo(int argc, char *argv[]) {
    if (argc != 2) {
        print("Usage: philo <max_number>\n", WHITE);
        return;
    }

    int maxPhilos = strToInt(argv[1]);
    if (maxPhilos <= 0 || maxPhilos > MAX_PHILOSOPHERS) {
        print("Invalid number of philosophers (must be between 1 and ", WHITE);
        print(MAX_PHILOSOPHERS, WHITE);
        print(")\n", WHITE);
        return;
    }

    semaphores = allocMemory(sizeof(char*) * maxPhilos);
    if(!semaphores) {
        print("Memory allocation failed\n", WHITE);
        return;
    }
    philosophers = allocMemory(sizeof(pid_t) * maxPhilos);
    if(!philosophers) {
        print("Memory allocation failed\n", WHITE);
        freeMemory(semaphores);
        return;
    }
    status = allocMemory(sizeof(int) * maxPhilos);
    if (!status){
        print("Memory allocation failed\n", WHITE);
        freeMemory(semaphores);
        freeMemory(philosophers);
        return;
    }
    semOpen(auxSem, 1);
    createPhilosophers();
    print("\nCommands: 'q' to quit, 'a' to add philosopher, 'r' to remove philosopher\n", WHITE);
    while (1) {
        char c;
        if (getC(&c) <= 0 || c == EOF) {
            killPhilosophers();
            semClose(auxSem);
            freeMemory(semaphores);
            freeMemory(philosophers);
            freeMemory(status);
        }
        switch (c) {
            case 'q':
                killPhilosophers();
                semClose(auxSem);
                freeMemory(semaphores);
                freeMemory(philosophers);
                freeMemory(status);
                return;
            case 'a':
                if (philosopherCount < maxPhilos) {
                    philosopherCount++;
                    print("Added philosopher. Total: ", WHITE);
                    print(philosopherCount, WHITE);
                    print("\n", WHITE);
                } else {
                    print("Max philosophers (", WHITE);
                    print(maxPhilos, WHITE);
                    print(") reached\n", WHITE);
                }
                break;
            case 'r':
                if (philosopherCount > INITIAL) {
                    philosopherCount--;
                    print("Removed philosopher. Total: ", WHITE);
                    print(philosopherCount, WHITE);
                    print("\n", WHITE);
                } else {
                    print("Minimum philosophers (", WHITE);
                    print(INITIAL, WHITE);
                    print(") reached\n", WHITE);
                }
                break;
            default:
                print("Invalid command. Use 'q', 'a', or 'r'\n", WHITE);
        }
    }
}