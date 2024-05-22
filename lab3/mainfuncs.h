#ifndef UNTITLED_MAINFUNCTIONS
#define UNTITLED_MAINFUNCTIONS

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#define REPEAT_COUNT 5

typedef struct {
    int firstVal;
    int secondVal;
} PAIR;

extern NODE* head;

void createChild();
void deleteLastChild();
void sendForbid(int);
void sendAllow(int);
void forbid();
void allow();
void forbidAndShow(int);

void printStatistics();
void pairCycle();
void takeStatistics();
void allowAll();

#endif
