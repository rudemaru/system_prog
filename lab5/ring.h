#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

extern int MAX_MES_COUNT;

typedef struct {
    u_int8_t type;
    u_int16_t hash;
    u_int8_t size;
    u_int8_t* data;
} MESSAGE;

typedef struct node {
    MESSAGE* message;
    struct node* next;
    struct node* prev;
} NODE;

typedef struct {
    NODE* ringHead;
    NODE* ringTail;
    int countAdded;
    int countDeleted;
} QUEUE;

void push(NODE**, NODE**);
void pop(NODE**, NODE**);

void initMes(MESSAGE*);
void printMes(MESSAGE*);