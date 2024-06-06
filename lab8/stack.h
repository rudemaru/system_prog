#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    void* el;
    struct Node* next;
} NODE;

void push(NODE**, void*);
void pop(NODE**);
char* topStr(NODE*);