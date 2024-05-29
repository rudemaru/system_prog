#include "stack.h"

void pushStack(STACKNODE** head, pthread_t threadId) {
    STACKNODE *new = (STACKNODE *) malloc(sizeof(STACKNODE));
    new->next = *head;
    new->threadId = threadId;
    *head = new;
}
void popStack(STACKNODE** head) {
    if(*head!=NULL) {
        STACKNODE *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}