#ifndef HASHMAP_H
#define HASHMAP_H
#include "../mm/mm.h"
#include "../include/lib.h"
typedef int64_t (*compareFunc)(void * e1, void * e2);
typedef struct HashMap * HashMapADT;

uint64_t hash( void * key);
struct Entry * createEntry( void * key, void * value);
HashMapADT create_hash_map(compareFunc cmpFunc);
void insert(HashMapADT hashMap,  void * key, void * value);
void * lookup(HashMapADT hashMap,  void * key);
void delete(HashMapADT hashMap,  void * key);
void free_hash_map(HashMapADT hashMap);


#endif