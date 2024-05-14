#include "mainfuncs.h"

PAIR pair = {0, 0};

int makeValues = 1;
int _00 = 0;
int _01 = 0;
int _10 = 0;
int _11 = 0;
int canShow = 1;

void createChild() {
    pid_t chpid = fork();
    if(chpid == 0) {
        signal(SIGALRM, takeStatistics);
        pairCycle();
    } else if (chpid == -1) {
        printf("Errno: %d\n", errno);
        exit(errno);
    } else {
        push(&head, chpid);
        printf("Process with pid:%d created.\n", chpid);
    }
}

void deleteLastChild() {
    kill(head->pid, SIGKILL);
    printf("Process C_%d with pid:%d deleted.\n", head->num, head->pid);
    pop(&head);
}

void forbid() {
    canShow = 0;
}

void allow() {
    canShow = 1;
}

void sendForbid(int procNum) {
    if (procNum == -1) {
        NODE *temp = head;
        while (temp) {
            kill(temp->pid, SIGUSR2);
            temp = temp->next;
        }
    } else {
        NODE *temp = head;
        while (temp && temp->num != procNum)
            temp = temp->next;
        if (temp)
            kill(temp->pid, SIGUSR2);
    }
}

void sendAllow(int procNum) {
    if (procNum == -1) {
        NODE *temp = head;
        while (temp) {
            kill(temp->pid, SIGUSR1);
            temp = temp->next;
        }
    } else {
        NODE *temp = head;
        while (temp && temp->num != procNum)
            temp = temp->next;
        if (temp)
            kill(temp->pid, SIGUSR1);
    }
}

void forbidAndShow(int procNum) {
    sendForbid(-1);
    NODE *temp = head;
    while (temp && temp->num != procNum)
        temp = temp->next;
    if (temp)
        kill(temp->pid, SIGINT);
    signal(SIGALRM, allowAll);
    alarm(5);
}

void pairCycle() {
    int currentRepeat = 0;
    while(1) {
        if(currentRepeat == REPEAT_COUNT) {
            currentRepeat = 0;
            if (canShow)
                printStatistics();
        }
        alarm(1);
        while (makeValues) {
            if (pair.firstVal == 0 && pair.secondVal == 0)
                pair.secondVal = 1;
            else if (pair.firstVal == 0 && pair.secondVal == 1) {
                pair.firstVal = 1;
                pair.secondVal = 0;
            } else if (pair.firstVal == 1 && pair.secondVal == 0)
                pair.secondVal = 1;
            else {
                pair.firstVal = 0;
                pair.secondVal = 0;
            }
        }
        makeValues = 1;
        currentRepeat++;
    }
}

void takeStatistics() {
    if (pair.firstVal == 0 && pair.secondVal == 0)
        _00++;
    else if (pair.firstVal == 0 && pair.secondVal == 1)
        _01++;
    else if (pair.firstVal == 1 && pair.secondVal == 0)
        _10++;
    else
        _11++;
    makeValues = 0;
}

void printStatistics() {
    printf("[%d] ppid:%d, {0,0} - %d, {0,1} - %d, {1,0} - %d, {1,1} - %d\n",
           getpid(), getppid(), _00, _01, _10, _11);
}

void allowAll() {
    sendAllow(-1);
}