#include "stack.h"

void push(NODE** head, pid_t pid) {
    NODE *new = (NODE *) malloc(sizeof(NODE));
    new->next = *head;
    new->pid = pid;
    new->num = size(*head) + 1;
    *head = new;
}
void pop(NODE** head) {
    if(*head!=NULL) {
        NODE *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
void print(NODE* head) {
    NODE *temp = head;
    while (temp) {
        printf("Process C_%d with pid = %d\n", temp->num, temp->pid);
        temp = temp->next;
    }
}
void erase(NODE** head) {
    while(*head) {
        NODE* temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
int size(NODE* head) {
    int size = 0;
    NODE* temp = head;
    while(temp) {
        size++;
        temp = temp->next;
    }
    return size;
}