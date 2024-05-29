#include "ring.h"

void push(NODE** head, NODE** tail) {
    if (*head != NULL) {
        NODE *temp = (NODE *) malloc(sizeof(NODE));
        temp->message = (MESSAGE*)malloc(sizeof(MESSAGE));
        initMes(temp->message);
        temp->next = *head;
        temp->prev = *tail;
        (*tail)->next = temp;
        (*head)->prev = temp;
        *tail = temp;
    } else {
        *head = (NODE *) malloc(sizeof(NODE));
        (*head)->message = (MESSAGE*)malloc(sizeof(MESSAGE));
        initMes((*head)->message);
        (*head)->prev = *head;
        (*head)->next = *head;
        *tail = *head;
    }
}
void pop(NODE** head, NODE** tail) {
    if (*head != NULL) {
        if (*head != *tail) {
            NODE *temp = *head;
            (*tail)->next = (*head)->next;
            *head = (*head)->next;
            (*head)->prev = *tail;
            free(temp);
        } else {
            free(*head);
            *head = NULL;
            *tail = NULL;
        }
    }
}

void initMes(MESSAGE* message) {
    message->type = 0;
    message->hash = 0;
    message->size = rand() % 257;
    message->data = (u_int8_t*)malloc(message->size*sizeof(u_int8_t));
    for (size_t i = 0; i < message->size; i++) {
        message->data[i] = rand() % 256;
        message->hash += message->data[i];
    }
    message->hash = ~message->hash;
}

void printMes(MESSAGE* mes) {
    printf("Message type: %d, hash: %d, size: %d, data: ", mes->type, mes->hash, mes->size);
    for(size_t i = 0; i<mes->size; i++)
        printf("%d", mes->data[i]);
    printf("\n");
}