#ifndef HASHMAP_H
#define HASHMAP_H
#include "../mm/mm.h"
#include "../include/lib.h"
#include "../include/process.h"
typedef int64_t (*compareFunc)(pid_t e1, pid_t e2);
typedef struct HashMap * HashMapADT;

uint64_t hash( pid_t key);
struct Entry * createEntry( pid_t key, PCB value);
HashMapADT create_hash_map(compareFunc cmpFunc);
void insert(HashMapADT hashMap, pid_t key, PCB value);
void * lookup(HashMapADT hashMap,  pid_t key);
void delete(HashMapADT hashMap,  pid_t key);
void free_hash_map(HashMapADT hashMap);


#endif