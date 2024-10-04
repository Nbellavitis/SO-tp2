//
// Created by Ivo Vilamowski on 04/10/2024.
//

#ifndef SO_TP2_QUEUE_H
#define SO_TP2_QUEUE_H
#include "../mm/mm.h"


typedef int64_t (*compareFunc)(void * e1, void * e2);
typedef queueCDT * queueADT;

queueADT newQueue();

int8_t queue(queueADT q, void * in);

void * dequeue(queueADT q);

int8_t remove(queueADT q, void * elem);

int8_t isEmpty(queueADT q);

void freeQueue(queueADT q);

//void toBegin(queueADT q);

//int8_t hasNext(queueADT q);

//int8_t next(queueADT q, void * out);

uint64_t size(queueADT q);


#endif //SO_TP2_QUEUE_H
