// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com
#include "hashMap.h"
#define TABLE_SIZE 2000
#include "../Drivers/include/videoDriver.h"
typedef struct Entry {
  pid_t key;
  PCB value;
  struct Entry *next;

} Entry;

typedef struct HashMap {
  Entry *buckets[TABLE_SIZE];
  compareFunc cmpFunc;
} HashMap;

uint64_t hash(pid_t key) { if(key < 0){
        return -key % TABLE_SIZE;
}
    return key % TABLE_SIZE; }

Entry *createEntry(pid_t key, PCB value) {
  Entry *newEntry = (Entry *)allocMemory(sizeof(Entry));
  if (newEntry == NULL) {
    drawWord(0xFF0000, "Error creating entry");
    return NULL;
  }
  newEntry->key = key;
  newEntry->value = value;
  newEntry->next = NULL;
  return newEntry;
}

HashMap *create_hash_map(compareFunc cmpFunc) {
  HashMap *hashMap = (HashMap *)allocMemory(sizeof(HashMap));
  if (hashMap == NULL) {
    drawWord(0xFF0000, "Error creating hashmap");
    return NULL;
  }
  for (int i = 0; i < TABLE_SIZE; i++) {
    hashMap->buckets[i] = NULL;
  }
  hashMap->cmpFunc = cmpFunc;
  return hashMap;
}

void insert(HashMap *hashMap, pid_t key, PCB value) {
  unsigned int index = hash(key);
  Entry *newEntry = createEntry(key, value);
  if(newEntry == NULL){
    return;
  }
  if (hashMap->buckets[index] == NULL) {
    hashMap->buckets[index] = newEntry;
  } else {

    Entry *current = hashMap->buckets[index];
    newEntry->next = current;
    hashMap->buckets[index] = newEntry;
  }
}

void *lookup(HashMap *hashMap, pid_t key) {
  unsigned int index = hash(key);
  Entry *current = hashMap->buckets[index];

  while (current != NULL) {
    if (hashMap->cmpFunc(current->key, key) == 0) {
      return current->value;
    }
    current = current->next;
  }
  return NULL;
}

void delete (HashMap *hashMap, pid_t key) {
  unsigned int index = hash(key);
  Entry *current = hashMap->buckets[index];
  Entry *previous = NULL;

  while (current != NULL && hashMap->cmpFunc(current->key, key) != 0) {
    previous = current;
    current = current->next;
  }

  if (current == NULL) {
    return;
  }

  if (previous == NULL) {

    hashMap->buckets[index] = current->next;
  } else {
    previous->next = current->next;
  }

  freeMemory(current);
}

void freeHashMap(HashMap *hashMap) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    Entry *current = hashMap->buckets[i];
    while (current != NULL) {
      Entry *next = current->next;
      freeMemory(current);
      current = next;
    }
  }
  freeMemory(hashMap);
}
