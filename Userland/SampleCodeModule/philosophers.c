// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "include/lib.h"
#include "include/usrSysCall.h"
#include <stdio.h>

#define TIME 30
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

typedef struct {
  pid_t pid;
  char *id;
  char **args;
  char *semName;
  char **descriptors;
  int status;
} Philosopher;

typedef struct {
  char *mutex;
  uint64_t philosopherCount;
  uint64_t maxPhilosophers;
  Philosopher **philosophers;
  int newPhilosopher;
  char *lastThinking;
  char *secondLast;
  char *lastHungry;
  int flag;
} philosData;

philosData data;

void killPhilo(int id);
void philosopher(int argc, char *argv[]);
void generateIdString(const char *prefix, int id, char *output, int maxLen);

int getLeftIndex(int id) {
  return (id + data.philosopherCount - 1) % data.philosopherCount;
}

int getRightIndex(int id) { return (id + 1) % data.philosopherCount; }
int getRightBlock(int id) {
  semWait(data.mutex);
  int right = (id + 1) % data.philosopherCount;
  semPost(data.mutex);
  return right;
}

Philosopher *createPhilo(int i) {
  Philosopher *newPhil = allocMemory(sizeof(Philosopher));
  if (!newPhil) {
    print(WHITE, "Memory allocation failed\n");
    semPost(data.mutex);
    return NULL;
  }

  char *id = allocMemory(ID_SIZE);
  if (!id) {
    print(WHITE, "Memory allocation failed\n");
    freeMemory(newPhil);
    semPost(data.mutex);
    return NULL;
  }
  intToStr(i, id);
  newPhil->id = id;

  char *semName = allocMemory(SEM_NAME_SIZE);
  if (!semName) {
    print(WHITE, "Memory allocation failed\n");
    freeMemory(id);
    freeMemory(newPhil);
    semPost(data.mutex);
    return NULL;
  }
  generateIdString("sem_", i, semName, SEM_NAME_SIZE);

  char **arg = allocMemory(3 * sizeof(char *));
  if (!arg) {
    print(WHITE, "Memory allocation failed\n");
    freeMemory(semName);
    freeMemory(id);
    freeMemory(newPhil);
    semPost(data.mutex);
    return NULL;
  }
  arg[0] = "philosopher";
  arg[1] = id;
  arg[2] = NULL;
  newPhil->args = arg;
  char **descriptors = allocMemory(2 * sizeof(char *));
  if (!descriptors) {
    print(WHITE, "Memory allocation failed\n");
    freeMemory(arg);
    freeMemory(semName);
    freeMemory(id);
    freeMemory(newPhil);
    semPost(data.mutex);
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
    semPost(data.mutex);
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
    semPost(data.mutex);
    return NULL;
  }

  newPhil->status = THINKING;
  return newPhil;
}

void printState(void) {
  semWait(data.mutex);
  uint64_t currentCount = data.philosopherCount;
  for (int i = 0; i < currentCount && i < data.maxPhilosophers; i++) {
    if (data.philosophers[i] != NULL) {
      print(WHITE, data.philosophers[i]->status == EATING ? "E" : ".");
      print(WHITE, " ");
    }
  }
  print(WHITE, "\n");
  semPost(data.mutex);
}

void takeFork(int id) {
  if (id % 2 == 0) {
    semWait(data.philosophers[id]->semName);
    semWait(data.philosophers[getRightBlock(id)]->semName);
  } else {
    semWait(data.philosophers[getRightBlock(id)]->semName);
    semWait(data.philosophers[id]->semName);
  }
}

void putFork(int id) {
  if (id % 2 == 0) {
    semPost(data.philosophers[getRightBlock(id)]->semName);
    semPost(data.philosophers[id]->semName);

  } else {
    semPost(data.philosophers[id]->semName);
    semPost(data.philosophers[getRightBlock(id)]->semName);
  }
}

void removePhilosopher() {
  semWait(data.mutex);
  if (data.philosopherCount <= INITIAL) {
    semPost(data.mutex);
    return;
  }
  semPost(data.mutex);
  semWait(data.secondLast);
  semWait(data.lastHungry);
  semWait(data.mutex);
  int id = data.philosopherCount - 1;
  if (data.philosophers[id]->status == THINKING) {
    semPost(data.mutex);
    takeFork(id);
    semWait(data.mutex);
  }
  killPhilo(id);
    data.philosopherCount--;
    semPost(data.philosophers[0]->semName);

  semPost(data.mutex);
  semPost(data.lastHungry);
  semPost(data.secondLast);

}

void philosopher(int argc, char *argv[]) {
  if (argc < 2) {
    print(WHITE, "Error: Philosopher needs ID argument\n");
    return;
  }
  int id = satoi(argv[1]);
  int flag = 0;
  while (1) {
    semWait(data.mutex);
    flag = 0;
    if (id == data.philosopherCount - 2) {
       flag = 1;
      semPost(data.mutex);
      semPost(data.secondLast);
      semWait(data.secondLast);
      semWait(data.mutex);
    }
    data.philosophers[id]->status = THINKING;
      if (id == data.philosopherCount - 1 && !flag) {
          semPost(data.mutex);
          semPost(data.lastHungry);
          semWait(data.lastHungry);
          semWait(data.mutex);
      }
    if (data.newPhilosopher == TRUE && data.philosopherCount - 1 == id && !flag) {
      semPost(data.mutex);
      semPost(data.lastThinking);
      semWait(data.lastThinking);
      semWait(data.mutex);
    }
    data.philosophers[id]->status = HUNGRY;
    semPost(data.mutex);
    callSleepMs(TIME);
    takeFork(id);
    semWait(data.mutex);
    data.philosophers[id]->status = EATING;
      if (id == data.philosopherCount - 1 && !flag) {
          semPost(data.mutex);
          semPost(data.lastHungry);
          semWait(data.lastHungry);
          semWait(data.mutex);
      }
    semPost(data.mutex);

    printState();

    callSleepMs(TIME);
    semWait(data.mutex);
    data.philosophers[id]->status = THINKING;
    semPost(data.mutex);

    putFork(id);

  }
}

void generateIdString(const char *prefix, int id, char *output, int maxLen) {
  if (!prefix || !output || maxLen <= 0)
    return;

  int i = 0;
  while (i < maxLen - 12 && prefix[i] != '\0') {
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
    data.philosophers[i] = createPhilo(i);
  }
}

void killPhilo(int id) {
  if (data.philosophers[id]->pid > 0) {
    print(0xFF0000,"anteultimo estado %d\n", data.philosophers[id-1]->status);
    print(0xFF0000,"ultimo estado %d\n", data.philosophers[id]->status);
    freeMemory(data.philosophers[id]->id);
    semClose(data.philosophers[id]->semName);
    freeMemory(data.philosophers[id]->semName);
    freeMemory(data.philosophers[id]->args);
    freeMemory(data.philosophers[id]->descriptors);
    killProcess(data.philosophers[id]->pid);
    freeMemory(data.philosophers[id]);
  }
}

void killPhilosophers() {
  semWait(data.mutex);
  if (!data.philosophers)
    return;
  for (int i = 0; i < data.philosopherCount; i++) {
    killPhilo(i);
    data.philosophers[i] = NULL;
  }
  semPost(data.mutex);
}

void addPhilosopher() {
  semWait(data.mutex);
  if (data.philosopherCount < data.maxPhilosophers) {
    data.newPhilosopher = TRUE;
    semPost(data.mutex);
    semWait(data.lastThinking);
    semWait(data.mutex);
    data.newPhilosopher = FALSE;
    data.philosophers[data.philosopherCount] =
        createPhilo(data.philosopherCount);
    data.philosopherCount++;
    semPost(data.mutex);
    semPost(data.lastThinking);
    semWait(data.mutex);
  }
  semPost(data.mutex);
}

void finish() {
  killPhilosophers();
  semClose(data.mutex);
  semClose(data.lastThinking);
  semClose(data.lastHungry);
  semClose(data.secondLast);
  freeMemory(data.philosophers);
}

void philo(int argc, char *argv[]) {
  if (argc != 2) {
    print(WHITE, "Usage: philo <max_number>\n");
    return;
  }
  data.mutex = "mutex";
  data.lastThinking = "lastThinking";
  data.lastHungry = "lastHungry";
  data.secondLast = "secondLast";
  data.flag = 0;
  data.newPhilosopher = FALSE;
  data.philosopherCount = INITIAL;
  data.maxPhilosophers = satoi(argv[1]);
  print(WHITE, "Max philosophers: %d\n", data.maxPhilosophers);
  if (data.maxPhilosophers <= INITIAL ||
      data.maxPhilosophers > MAX_PHILOSOPHERS) {
    print(WHITE, "Invalid number of philosophers (must be between %d and %d)\n",
          INITIAL, MAX_PHILOSOPHERS);
    return;
  }
  data.philosophers = allocMemory(sizeof(Philosopher) * data.maxPhilosophers);
  if (!data.philosophers) {
    print(WHITE, "Memory allocation failed\n");
    return;
  }

  if (semOpen(data.mutex, 1) == 0) {
    print(WHITE, "Failed to create auxiliary semaphore\n");
    freeMemory(data.philosophers);
    return;
  }
  if (semOpen(data.lastThinking, 0) == 0) {
    print(WHITE, "Failed to create auxiliary semaphore\n");
    semClose(data.mutex);
    freeMemory(data.philosophers);
    return;
  }
  if (semOpen(data.secondLast, 0) == 0) {
    print(WHITE, "Failed to create auxiliary semaphore\n");
    semClose(data.mutex);
    semClose(data.lastThinking);
    freeMemory(data.philosophers);
    return;
  }
  if (semOpen(data.lastHungry, 0) == 0) {
    print(WHITE, "Failed to create auxiliary semaphore\n");
    semClose(data.mutex);
    semClose(data.lastHungry);
    freeMemory(data.philosophers);
    return;
  }
  createPhilosophers(INITIAL);
  print(WHITE, "\nCommands: 'q' to quit, 'a' to add philosopher, 'r' to remove "
               "philosopher\n");

  char c = 0;
  while (getC(&c) >= 0 && c != 'q' && data.philosopherCount > 0 && c != EOF) {
    switch (c) {
    case 'q':
      finish();
      return;
    case 'a':
      addPhilosopher();
      break;
    case 'r':
      removePhilosopher();
      break;
    }
  }
  finish();
  return;
}
