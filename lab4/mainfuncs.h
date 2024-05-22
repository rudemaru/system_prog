#pragma once
#include "ring.h"
#include "stack.h"
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SEND_SEM_NAME "/send_sem"
#define RECEIVE_SEM_NAME "/receive_sem"
#define QUEUE_ACCESS_SEM_NAME "/queue_sem"
#define SHARED_MEMORY_NAME "/queue_messages"

extern int continuing;

void sendingMessages();
void receivingMessages();

void changeContinuingStatus();