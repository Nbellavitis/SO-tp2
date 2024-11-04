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

//char * auxSem = "sem_";
//
//int * status;
//uint64_t philosopherCount = INITIAL;
//char ** semaphores;
//pid_t * philosophers;
//
//int getLeftIndex(int id) {
//    return (id + philosopherCount - 1) % philosopherCount;
//}
//
//int getRightIndex(int id) {
//    return (id + 1) % philosopherCount;
//}
//
//bool isValidState(int state) {
//    return state == THINKING || state == HUNGRY || state == EATING;
//}
//
//bool isValidId(int id) {
//    return id >= 0 && id < philosopherCount;
//}
//
//void printState(void) {
//    semWait(auxSem);
//    uint64_t current_count = philosopherCount;
//    for (int i = 0; i < current_count && i < MAX_PHILOSOPHERS; i++) {
//        if (isValidState(status[i])) {
//            print(WHITE, status[i] == EATING ? "E" : ".");
//            print(WHITE, " ");
//        }
//    }
//    print(WHITE, "\n");
//    semPost(auxSem);
//}
//
//void take_fork(int id) {
//    if (!isValidId(id)) return;
//
//    semWait(auxSem);
//    if (!isValidState(status[id])) {
//        print(WHITE, "Invalid philosopher state\n");
//        semPost(auxSem);
//        return;
//    }
//    status[id] = HUNGRY;
//    semPost(auxSem);
//
//    if (id % 2 == 0) {
//        semWait(semaphores[id]);
//        semWait(semaphores[getRightIndex(id)]);
//    } else {
//        semWait(semaphores[getRightIndex(id)]);
//        semWait(semaphores[id]);
//    }
//}
//
//void put_fork(int id) {
//    if (!isValidId(id)) return;
//
//    if (id % 2 == 0) {
//        semPost(semaphores[getRightIndex(id)]);
//        semPost(semaphores[id]);
//    } else {
//        semPost(semaphores[id]);
//        semPost(semaphores[getRightIndex(id)]);
//    }
//    semWait(auxSem);
//    status[id] = THINKING;
//    semPost(auxSem);
//}
//
//void removePhilosopher(int id) {
//    if (!isValidId(id)) return;
//
//    semWait(auxSem);
//    if (status[id] == EATING) {
//        put_fork(id);
//    }
//
//    killProcess(philosophers[id]);
//    semClose(semaphores[id]);
//
//    for (int i = id; i < philosopherCount - 1; i++) {
//        philosophers[i] = philosophers[i + 1];
//        semaphores[i] = semaphores[i + 1];
//        status[i] = status[i + 1];
//    }
//
//    philosopherCount--;
//    semPost(auxSem);
//}
//
//void *philosopher(int argc, char *argv[]) {
//    if (argc < 2) {
//        print(WHITE, "Error: Philosopher needs ID argument\n");
//        return NULL;
//    }
//
//    int id = strToInt(argv[1]);
//    if (!isValidId(id)) {
//        print(WHITE, "Error: Invalid philosopher ID\n");
//        return NULL;
//    }
//
//    while (1) {
//        semWait(auxSem);
//        if (!isValidId(id)) {
//            semPost(auxSem);
//            return ;
//        }
//        status[id] = THINKING;
//        semPost(auxSem);
//
//        for (volatile int i = 0; i < TIME; i++);
//
//        take_fork(id);
//
//        semWait(auxSem);
//        if (!isValidId(id)) {
//            semPost(auxSem);
//            return ;
//        }
//        status[id] = EATING;
//        semPost(auxSem);
//
//        printState();
//        for (volatile int i = 0; i < TIME; i++);
//
//        put_fork(id);
//    }
//    return ;
//}
//
//void generateIdString(const char* prefix, int id, char* output, int maxLen) {
//    if (!prefix || !output || maxLen <= 0) return;
//
//    int i = 0;
//    while (prefix[i] != '\0' && i < maxLen - 12) {
//        output[i] = prefix[i];
//        i++;
//    }
//
//    char numStr[11];
//    intToStr(id, numStr);
//
//    int j = 0;
//    while (numStr[j] != '\0' && i < maxLen - 1) {
//        output[i++] = numStr[j++];
//    }
//
//    output[i] = '\0';
//}
//
//void createPhilosophers(int maxPhilos) {
//    semWait(auxSem);
//    for (int i = 0; i < maxPhilos; i++) {
//        char id[ID_SIZE];
//        intToStr(i, id);
//
//        char semName[SEM_NAME_SIZE];
//        generateIdString("sem_", i, semName, SEM_NAME_SIZE);
//
//        char *arg[] = {"philosopher", id, NULL};
//        char **descriptors = allocMemory(2 * sizeof(char *));
//        descriptors[0] = "tty";
//        descriptors[1] = "tty";
//        philosophers[i] = createProcess((uint64_t)philosopher, 1, 2, arg, descriptors);
//        if (philosophers[i] < 0) {
//            print(WHITE, "Failed to create philosopher ");
//            print(WHITE, i);
//            print(WHITE, "\n");
//            semPost(auxSem);
//            return;
//        }
//
//        semaphores[i] = semOpen(semName, 1);
//        if (semaphores[i] == NULL) {
//            print(WHITE, "Failed to create semaphore ");
//            print(WHITE, i);
//            print(WHITE, "\n");
//            killProcess(philosophers[i]);
//            semPost(auxSem);
//            return;
//        }
//
//        status[i] = THINKING;
//    }
//    semPost(auxSem);
//}
//
//void killPhilosophers() {
//    if (!philosophers || !semaphores) return;
//
//    semWait(auxSem);
//    for (int i = 0; i < philosopherCount; i++) {
//        if (status[i] == EATING) {
//            put_fork(i);
//        }
//        if (philosophers[i] > 0) {
//            killProcess(philosophers[i]);
//            philosophers[i] = -1;
//        }
//        if (semaphores[i] != NULL) {
//            semClose(semaphores[i]);
//            semaphores[i] = NULL;
//        }
//    }
//    semPost(auxSem);
//}

void philo(int argc, char *argv[]) {
    return;
//    if (argc != 2) {
//        print(WHITE, "Usage: philo <max_number>\n");
//        return;
//    }
//
//    int maxPhilos = strToInt(argv[1]);
//    if (maxPhilos <= 0 || maxPhilos > MAX_PHILOSOPHERS) {
//        print(WHITE, "Invalid number of philosophers (must be between 1 and ");
//        print(WHITE, MAX_PHILOSOPHERS);
//        print(WHITE, ")\n");
//        return;
//    }
//
//    semaphores = allocMemory(sizeof(char *) * maxPhilos);
//    if (!semaphores) {
//        print(WHITE, "Memory allocation failed\n");
//        return;
//    }
//    philosophers = allocMemory(sizeof(pid_t) * maxPhilos);
//    if (!philosophers) {
//        print(WHITE, "Memory allocation failed\n");
//        freeMemory(semaphores);
//        return;
//    }
//    status = allocMemory(sizeof(int) * maxPhilos);
//    if (!status) {
//        print(WHITE, "Memory allocation failed\n");
//        freeMemory(semaphores);
//        freeMemory(philosophers);
//        return;
//    }
//
//    if (semOpen(auxSem, 1) == NULL) {
//        print(WHITE, "Failed to create auxiliary semaphore\n");
//        freeMemory(status);
//        freeMemory(semaphores);
//        freeMemory(philosophers);
//        return;
//    }
//
//    createPhilosophers(maxPhilos);
//    print(WHITE, "\nCommands: 'q' to quit, 'a' to add philosopher, 'r' to remove philosopher\n");
//
//    while (1) {
//        char c;
//        if (getC(&c) <= 0 || c == EOF) {
//            killPhilosophers();
//            semClose(auxSem);
//            freeMemory(semaphores);
//            freeMemory(philosophers);
//            freeMemory(status);
//            return;
//        }
//
//        switch (c) {
//            case 'q':
//                killPhilosophers();
//                semClose(auxSem);
//                freeMemory(semaphores);
//                freeMemory(philosophers);
//                freeMemory(status);
//                return;
//            case 'a':
//                semWait(auxSem);
//                if (philosopherCount < maxPhilos) {
//                    char id[ID_SIZE];
//                    intToStr(philosopherCount, id);
//                    char semName[SEM_NAME_SIZE];
//                    generateIdString("sem_", philosopherCount, semName, SEM_NAME_SIZE);
//
//                    char *arg[] = {"philosopher", id, NULL};
//                    char **descriptors = allocMemory(2 * sizeof(char *));
//                    descriptors[0] = "tty";
//                    descriptors[1] = "tty";
//                    pid_t newPhil = createProcess((uint64_t) philosopher, 1,  2, arg,descriptors);
//                    if (newPhil < 0) {
//                        print(WHITE, "Failed to create new philosopher\n");
//                        semPost(auxSem);
//                        break;
//
//                    }
//                }
//        }
//    }
}
