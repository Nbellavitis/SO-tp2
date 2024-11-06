#include "queue.h"
#include "../Drivers/include/videoDriver.h"
typedef struct tNode{
    void * element;
    struct tNode * next;
}tNode;

typedef tNode * tList;

typedef struct queueCDT{
    compareFunction cmpFunc;
    tList first;
    tList current;
    size_t amount;
    tList iter;
}queueCDT;

queueADT createQueue(compareFunction cmpFunc){
    queueADT newQueue = (queueCDT *) allocMemory(sizeof(queueCDT));
    if(newQueue == NULL){
        drawWord(0xFF0000,"Error creating queue");
        return NULL;
    }
    newQueue->cmpFunc = cmpFunc;
    newQueue->amount = 0;
    newQueue->first = NULL;
    newQueue->current = NULL;
    newQueue->iter = NULL;
    return newQueue;
}

tList createNode(void * element){
    tList newNode = (tList) allocMemory(sizeof(tNode));
    if(newNode == NULL){
        drawWord(0xFF0000,"Error creating node");
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
        queue->first = newNode;
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
    void * auxElem=auxNode->element;
    freeMemory(auxNode);
    return auxElem;
}


tList removeFromList(tList list, compareFunction cmp, void * elem, int8_t *flag){
    if (list == NULL) {
        *flag = 0;
        return list;
    }

    if (cmp(elem, list->element) == 0) {
        tList aux = list;
        list = aux->next;
        freeMemory(aux);
        *flag = 1;
        return list;
    }

    list->next = removeFromList(list->next, cmp, elem, flag);
    return list;
}

int8_t remove(queueADT queue, void * elem) {
    if (queue->first == NULL) {
        return 0;
    }

    int8_t flag;
    queue->first = removeFromList(queue->first, queue->cmpFunc, elem, &flag);
    queue->amount -= flag;
    return flag;
}

int8_t isEmpty(queueADT queue){
    if(queue->first == NULL){
        return 1;
    }
    return 0;
}


void freeQueueRec(tList list){
    if(list == NULL){
        return;
    }
    freeQueueRec(list->next);
    freeMemory(list);
    return;
}

void freeQueue(queueADT queue){
    if(queue == NULL){
        return;
    }
    freeQueueRec(queue->first);
    freeMemory(queue);
}



uint64_t sizeQ(queueADT queue){
    return queue->amount;
}

void toBegin(queueADT queue){
    queue->iter=queue->first;
}

int8_t hasNext(queueADT queue){
    return (queue->iter != NULL);
}

void * next(queueADT queue){
    if(hasNext(queue)){
        tList aux = queue->iter;
        queue->iter = queue->iter->next;
        return aux->element;
    }
    return NULL;
}







