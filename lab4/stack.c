#include "stack.h"

void pushStack(STACKNODE** head, pid_t pid) {
    STACKNODE *new = (STACKNODE *) malloc(sizeof(STACKNODE));
    new->next = *head;
    new->pid = pid;
    *head = new;
}
void popStack(STACKNODE** head) {
    if(*head!=NULL) {
        STACKNODE *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}