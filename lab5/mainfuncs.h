#pragma once
#include "ring.h"
#include "stack.h"
#include "memory.h"
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/signal.h>

#define _POSIX_C_SOURCE 199309L

#define PRODUCE_SEM_NAME "/fill_sem"
#define CONSUME_SEM_NAME "/extract_sem"
#define QUEUE_ACCESS_SEM_NAME "/queue_sem"

extern int continuing;
extern QUEUE* queue;

void *produceMessages();
void *consumeMessages();

void changeContinuingStatus();