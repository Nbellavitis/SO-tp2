#include "hashMap.h"
#define TABLE_SIZE 2000

typedef struct Entry {
    void * key;          
    void * value;          
    struct Entry *next;
 
} Entry;


typedef struct HashMap {
    Entry *buckets[TABLE_SIZE];
    compareFunc cmpFunc; 
} HashMap;

uint64_t hash( void * key) {
    return (* (uint64_t * ) key) % TABLE_SIZE;
}


Entry *createEntry( void * key, void * value) {
    Entry *newEntry = (Entry *)allocMemory(sizeof(Entry));
    newEntry->key = key; 
    newEntry->value = value;
    newEntry->next = NULL;
    return newEntry;
}


HashMap *create_hash_map(compareFunc cmpFunc) {
    HashMap *hashMap = (HashMap *)allocMemory(sizeof(HashMap));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashMap->buckets[i] = NULL; 
    }
    hashMap->cmpFunc=cmpFunc;
    return hashMap;
}


void insert(HashMap *hashMap,  void * key, void * value) {
    unsigned int index = hash(key);
    Entry *newEntry = createEntry(key, value);

    if (hashMap->buckets[index] == NULL) {
        hashMap->buckets[index] = newEntry;
    } else {
   
        Entry *current = hashMap->buckets[index];
        newEntry->next = current;
        hashMap->buckets[index] = newEntry;
    }
}

void * lookup(HashMap *hashMap,  void *key) {
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

void delete(HashMap *hashMap,  void *key) {
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


void freeHashMap(HashMap * hashMap) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry * current = hashMap->buckets[i];
        while (current != NULL) {
            Entry *next = current->next;
            freeMemory(current);      
            current = next;
        }
    }
    freeMemory(hashMap); 
}

