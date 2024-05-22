#include "ring.h"

void push(QUEUE* queue) {
    if(queue->currentPlaceToWrite > END_PLACE_TO_WRITE)                        // Установка места добавления.
        queue->currentPlaceToWrite = START_PLACE_TO_WRITE;
    NODE *temp = (NODE *)((uintptr_t)queue + queue->currentPlaceToWrite);      // Инициализация памяти.
    temp->message = ((uintptr_t)queue + queue->currentPlaceToWrite + sizeof(NODE));
    initMes((MESSAGE*)temp->message);                                          // Инициализация сообщения.
    if (queue->ringHead != 0) {
        temp->next = queue->ringHead;                                          // Добавление в кольцо.
        temp->prev = queue->ringTail;
        ((NODE*)(queue->ringHead+(uintptr_t)queue))->prev = (uintptr_t)temp - (uintptr_t)queue;
        ((NODE*)(queue->ringTail+(uintptr_t)queue))->next = (uintptr_t)temp - (uintptr_t)queue;
        queue->ringTail = (uintptr_t)temp - (uintptr_t)queue;
    } else {
        queue->ringHead = (uintptr_t)temp - (uintptr_t)queue;
        queue->ringTail = (uintptr_t)temp - (uintptr_t)queue;
        temp->next = (uintptr_t)temp - (uintptr_t)queue;
        temp->prev = (uintptr_t)temp - (uintptr_t)queue;
    }
    queue->currentPlaceToWrite += STEP_TO_WRITE;                                   
}
void pop(QUEUE* queue) {
    if (queue->ringHead != 0) {
        if (queue->ringHead != queue->ringTail) {
            NODE *temp = (NODE *) (queue->ringHead + (uintptr_t) queue);      // Удаление.
            ((NODE *) (queue->ringTail + (uintptr_t) queue))->next = temp->next;
            ((NODE *) (temp->next + (uintptr_t) queue))->prev = queue->ringTail;
            queue->ringHead = temp->next;
        } else {
            queue->ringHead = 0;
            queue->ringTail = 0;
        }
    }
}

void initMes(MESSAGE* message) {
    message->type = 0;                                    
    message->hash = 0;
    message->size = rand() % 257;
    for (size_t i = 0; i < message->size; i++) {
        message->data[i] = rand() % 256;
        message->hash += message->data[i];                // Для хэша.
    }
    message->hash = ~message->hash;                       // Метод дополнения.
}

void printMes(MESSAGE* mes) {
    printf("Message type: %d, hash: %d, size: %d, data: ", mes->type, mes->hash, mes->size);
    for(size_t i = 0; i<mes->size; i++)
        printf("%d", mes->data[i]);
    printf("\n");
}