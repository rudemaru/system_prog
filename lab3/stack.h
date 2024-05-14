#ifndef UNTITLED_STACK
#define UNTITLED_STACK

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

typedef struct node {
    int num;
    pid_t pid;
    struct node* next;
} NODE;

void push(NODE**, pid_t);
void pop(NODE**);
void print(NODE*);
void erase(NODE**);
int size(NODE*);

#endif
