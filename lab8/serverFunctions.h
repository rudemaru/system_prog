#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include "stack.h"
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define SERVER_MESSAGE "Welcome to the training server 'My server'"

extern char* defaulDir;
extern char* helloString;

void changeDirectory(NODE **stackDir, char *buf);
void listDirectory(NODE *stackDir, char **result);
void *handle_client(void *arg);

char* readHello(char*);
char* plusString(char*, char*);