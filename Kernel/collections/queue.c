//
// Created by Ivo Vilamowski on 04/10/2024.
//
#include "queue.h"

typedef struct tNode{
    void * element;
    tNode * next;
}tNode;

typedef tNode * tList;

typedef struct queueCDT{
    compareFunc cmpFunc;
    tList first;
    tList current;
    size_t amount;
}queueCDT;

queueADT createQueue(compareFunc cmpFunc){
    queueADT newQueue = (queueCDT *) allocMemory(sizeof(queueCDT));
    if(newQueue == NULL){
        return NULL;
    }
    newQueue->cmpFunc = cmpFunc;
    newQueue->amount = 0;
    newQueue->first = NULL;
    newQueue->current = NULL;
    return newQueue;
}

tList createNode(void * element){
    tList newNode = (tList) allocMemory(sizeof(tNode));
    if(newNode == NULL){
        return NULL;
    }
    newNode->element = element;
    newNode->next = NULL;
    return newNode;
}

int8_t queue(queueADT queue, void * in){
    tList newNode = createNode(in);
    if(newNode == NULL){
        return 0;
    }
    if(queue->first == NULL){
        queue->first == newNode;
    }
    queue->current->next = newNode;
    queue->current = newNode;
    queue->amount++;
    return 1;
}

void * dequeue(queueADT queue){
    if(queue->first == NULL){
        return NULL;
    }
    if(queue->first == queue->current){
        queue->current = NULL;
    }
    tList auxNode = queue->first;
    queue->first= auxNode->next;
    queue->amount--;
    return auxNode->element;
}

int8_t remove(queueADT queue, void * elem){
    if(queue->first == NULL){
        return 0;
    }
    tList aux = queue->first;
    size_t flag = removeFromList(aux, queue->cmpFunc, elem);
    queue->amount -= flag;
    return flag;
}

int8_t removeFromList(tList list, compareFunc cmp, void * elem){
    if(list == NULL){
        return 0;
    }
    if(cmp(elem,list->element) == 0){
        tList aux = list->element;
        list = aux->next;
        return 1;
    }
    return removeFromList(list->next,cmp,elem);
}

int8_t isEmpty(queueADT queue){
    if(queue->first == NULL){
        return 1;
    }
    return 0;
}


void freeQueue(queueADT queue){
    if(queue == NULL){
        return;
    }
    freeQueueRec(queue->first);
    freeMemory(queue);
}

void freeQueueRec(tList list){
    if(list == NULL){
        return;
    }
    freeQueueRec(list->next);
    free(list);
    return;
}

uint64_t size(queueADT queue){
    return queue->amount;
}








