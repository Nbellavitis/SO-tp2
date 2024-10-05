//
// Created by Ivo Vilamowski on 04/10/2024.
//

#ifndef SO_TP2_QUEUE_H
#define SO_TP2_QUEUE_H
#include "../mm/mm.h"


typedef int64_t (*compareFunc)(void * e1, void * e2);
typedef struct queueCDT * queueADT;
queueADT createQueue(compareFunc cmpFunc);

int8_t queue(queueADT queue, void * in);

void * dequeue(queueADT queue);

int8_t remove(queueADT queue, void * elem);

int8_t isEmpty(queueADT queue);

void freeQueue(queueADT queue);

void toBegin(queueADT queue);

int8_t hasNext(queueADT queue);

void * next(queueADT queue);

uint64_t size(queueADT queue);


#endif //SO_TP2_QUEUE_H
