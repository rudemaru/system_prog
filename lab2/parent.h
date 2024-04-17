#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

int compare(const void *a, const void *b);
char* scanEnvp(char* envp[]);
void setName(char** name, int num);