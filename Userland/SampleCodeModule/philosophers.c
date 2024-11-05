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
//char * lastEating = "sem_lastEating";
//char * almostLastThinking = "sem_almostLastThinking";

typedef struct {
    pid_t pid;
    char * id;
    char ** args;
    char * semName;
    char ** descriptors;
    int status;
    int forkAmount;
} Philosopher;

typedef struct{
    char * mutex;
    uint64_t philosopherCount;
    uint64_t maxPhilosophers;
    Philosopher ** philosophers;
} philosData;

typedef philosData * philosDataPtr;
philosDataPtr data = NULL;




void philosopher(int argc, char *argv[]);
void generateIdString(const char* prefix, int id, char* output, int maxLen);

int getLeftIndex(int id) {
    return (id + data->philosopherCount - 1) % data->philosopherCount;
}

int getRightIndex(int id) {
    return (id + 1) % data->philosopherCount;
}

bool isValidState(int state) {
    return state == THINKING || state == HUNGRY || state == EATING;
}

bool isValidId(int id) {
    return id >= 0 && id < data->philosopherCount;
}

Philosopher * createPhilo(int i){
    semWait(data->mutex);
    Philosopher* newPhil = allocMemory(sizeof(Philosopher));
    if (!newPhil) {
        print(WHITE, "Memory allocation failed\n");
        semPost(data->mutex);
        return NULL;
    }

    char* id = allocMemory(ID_SIZE);
    if(!id){
        print(WHITE, "Memory allocation failed\n");
        freeMemory(newPhil);
        semPost(data->mutex);
        return NULL;
    }
    intToStr(i, id);
    newPhil->id = id;

    char* semName = allocMemory(SEM_NAME_SIZE);
    if(!semName){
        print(WHITE, "Memory allocation failed\n");
        freeMemory(id);
        freeMemory(newPhil);
        semPost(data->mutex);
        return NULL;
    }
    generateIdString("sem_", i, semName, SEM_NAME_SIZE);

    char** arg = allocMemory(3 * sizeof(char*));
    if(!arg){
        print(WHITE, "Memory allocation failed\n");
        freeMemory(semName);
        freeMemory(id);
        freeMemory(newPhil);
        semPost(data->mutex);
        return NULL;
    }
    arg[0] = "philosopher";
    arg[1] = id;
    arg[2] = NULL;

    char** descriptors = allocMemory(2 * sizeof(char*));
    if(!descriptors){
        print(WHITE, "Memory allocation failed\n");
        freeMemory(arg);
        freeMemory(semName);
        freeMemory(id);
        freeMemory(newPhil);
        semPost(data->mutex);
        return NULL;
    }

    descriptors[0] = "tty";
    descriptors[1] = "tty";
    newPhil->descriptors = descriptors;

    if (semOpen(semName, 1) == 0) {
        print(WHITE, "Failed to create semaphore ");
        print(WHITE, "%d", i);
        print(WHITE, "\n");
        freeMemory(descriptors);
        freeMemory(arg);
        freeMemory(semName);
        freeMemory(id);
        freeMemory(newPhil);
        semPost(data->mutex);
        return NULL;
    }
    newPhil->semName = semName;

    newPhil->pid = createProcess((uint64_t)philosopher, 0, 2, arg, descriptors);
    if (newPhil->pid < 0) {
        print(WHITE, "Failed to create philosopher ");
        print(WHITE, "%d", i);
        print(WHITE, "\n");
        killProcess(newPhil->pid);
        freeMemory(descriptors);
        freeMemory(arg);
        freeMemory(semName);
        freeMemory(id);
        freeMemory(newPhil);
        semPost(data->mutex);
        return NULL;
    }

    newPhil->status = THINKING;
    newPhil->forkAmount = 0;
    semPost(data->mutex);
    return newPhil;
}


void printState(void) {
    semWait(data->mutex);
    uint64_t current_count = data->philosopherCount;
    for (int i = 0; i < current_count && i < data->maxPhilosophers; i++) {
        if (data->philosophers[i] != NULL && isValidState(data->philosophers[i]->status)) {
            print(WHITE, data->philosophers[i]->status == EATING ? "E" : ".");
            print(WHITE, " ");
        }
    }
    print(WHITE, "\n");
    semPost(data->mutex);
}

void take_fork(int id) {

    semWait(data->mutex);
    if (!isValidId(id)){
        semPost(data->mutex);
        return;
    }
    if (!isValidState(data->philosophers[id]->status)) {
        print(WHITE, "Invalid philosopher state\n");
        semPost(data->mutex);
        return;
    }
    data->philosophers[id]->status = HUNGRY;
    semPost(data->mutex);

    if (id % 2 == 0) {
        semWait(data->philosophers[id]->semName);
        semWait(data->philosophers[getRightIndex(id)]->semName);
    } else {
        semWait(data->philosophers[getRightIndex(id)]->semName);
        semWait(data->philosophers[id]->semName);
    }
    semWait(data->mutex);
    data->philosophers[id]->status = EATING;
    if(id == data->philosopherCount - 1){
//        semPost(auxSem);
//        semWait(lastEating);
    } else{
        semPost(data->mutex);
    }
}

void put_fork(int id) {
    if (!isValidId(id)){
        semPost(data->mutex);
        return;
    }

    if (id % 2 == 0) {
        semPost(data->philosophers[getRightIndex(id)]->semName);
        semPost(data->philosophers[id]->semName);
    } else {
        semPost(data->philosophers[id]->semName);
        semPost(data->philosophers[getRightIndex(id)]->semName);
    }
    semWait(data->mutex);
    data->philosophers[id]->status = THINKING;
//    if(id == philosopherCount - 1){
//        semPost(lastEating);
//    }
    semPost(data->mutex);

}


void checkEat(int id){
    if (data->philosophers[id]->status == EATING) {
        semPost(data->mutex);
        put_fork(id);
        semWait(data->mutex);
    }
}

void removePhilosopher(int id) {
    //si estaba comiendo, dejo los tenedores y lo borro
    //si estaba pensando, lo borro
    //si estaba hambriento, dejo el tenedor izquierdo y lo borro
    //si el de la izquierda estaba comiendo, deja el tendeor y lo borra
    //si el de la izquierda estaba pensando, lo borra
    //si el de la izquierda estaba hambriento, deja el tenedor derecho  y lo borra
    semWait(data->mutex);
    checkEat(id);
    checkEat(getLeftIndex(id));
//USAR killPhilo
    killProcess(data->philosophers[id]->pid);
    semPost(data->mutex);
    semClose(data->philosophers[id]->semName);
    data->philosopherCount--;
    semPost(data->mutex);
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
        semWait(data->mutex);

//        if(id == philosopherCount - 2){
//            semPost(auxSem);
//            semWait(almostLastThinking);
//            semPost(almostLastThinking);
//            semWait(auxSem);
//        }
        data->philosophers[id]->status = THINKING;
        semPost(data->mutex);

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

void createPhilosophers(int count) {
    for (int i = 0; i < count; i++) {
        data->philosophers[i] = createPhilo(i);
    }
}


void killPhilo(Philosopher* philo) {
    if (!isValidId(philo->id)) return;

    semWait(data->mutex);
    if (philo->pid > 0) {
        killProcess(philo->pid);
        freeMemory(philo->id);
        freeMemory(philo->semName);
        freeMemory(philo->args);
        freeMemory(philo->descriptors);
        freeMemory(philo);}
    semPost(data->mutex);

}

void killPhilosophers() {
    if (!data->philosophers) return;
    for (int i = 0; i < data->philosopherCount; i++) {
        killPhilo(data->philosophers[i]);
        data->philosophers[i] = NULL;
    }
}

void philo(int argc, char *argv[]) {
    if (argc != 2) {
        print(WHITE, "Usage: philo <max_number>\n");
        return;
    }
    philosDataPtr auxData = allocMemory(sizeof(philosData));
    if(!auxData){
        print(WHITE, "Memory allocation failed\n");
        return;
    }
    auxData->mutex = "mutex";
    auxData->philosopherCount = INITIAL;
    auxData->maxPhilosophers = satoi(argv[1]);
    print (WHITE, "Max philosophers: %d\n", auxData->maxPhilosophers);
    if (auxData->maxPhilosophers <= 0 || auxData->maxPhilosophers > MAX_PHILOSOPHERS) {
        //EL 5 debe ser INITIAL pero me dio paja
        print(WHITE, "Invalid number of philosophers (must be between 5 and ");
        print(WHITE, "%d", MAX_PHILOSOPHERS);
        print(WHITE, ")\n");
        return;
    }
    auxData->philosophers = allocMemory(sizeof(Philosopher) * auxData->maxPhilosophers);
    if (!auxData->philosophers) {
        print(WHITE, "Memory allocation failed\n");
        freeMemory(auxData);
        return;
    }

    if (semOpen(auxData->mutex, 1) == 0) {
        print(WHITE, "Failed to create auxiliary semaphore\n");
        freeMemory(auxData->philosophers);
        freeMemory(auxData);
        return;
    }

//    if(semOpen(lastEating, 1) == 0){
//        print(WHITE, "Failed to create lastEating semaphore\n");
//        freeMemory(philosophers);
//        semClose(auxSem);
//        return;
//    }
//    if(semOpen(almostLastThinking, 1) == 0){
//        print(WHITE, "Failed to create almostLastThinking semaphore\n");
//        freeMemory(philosophers);
//        semClose(auxSem);
//        semClose(lastEating);
//        return;
//    }
    data = auxData;

    createPhilosophers(INITIAL);
    print(WHITE, "\nCommands: 'q' to quit, 'a' to add philosopher, 'r' to remove philosopher\n");

    char c = 0;
    while (getC(&c) >= 0 && c != 'q' && data->philosopherCount > 0 && c != EOF) {
        switch (c) {
            case 'q':
                killPhilosophers();
                semClose(data->mutex);
                //semClose(lastEating);
                freeMemory(data->philosophers);
                return;
            case 'a':
                semWait(data->mutex);
                if (data->philosopherCount < data->maxPhilosophers ) {
                    data->philosophers[data->philosopherCount] = createPhilo(data->philosopherCount);
                    data->philosopherCount ++;
                }
                semPost(data->mutex);
                break;
            case 'r':
                semWait(data->mutex);
                if (data->philosopherCount > INITIAL) {
                  semPost(data->mutex);
                    removePhilosopher(data->philosopherCount - 1);
                }else{
                    semPost(data->mutex);
                }

                break;
        }
    }
    killPhilosophers();
    semClose(data->mutex);
    freeMemory(data->philosophers);
    freeMemory(data);
    return;
}


