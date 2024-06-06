#include "stack.h"

void push(NODE** head, void* el) {
    NODE *new = (NODE *) malloc(sizeof(NODE));
    new->next = *head;
    new->el = el;
    *head = new;
}
void pop(NODE** head) {
    if(*head!=NULL) {
        NODE *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
char* topStr(NODE* head) {
    return (char*)head->el;
}