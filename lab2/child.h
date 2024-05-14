#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

extern char **environ;

#define VAR_NUM 9

char** getFileVariables(const char* fname);
int includeString(const char* str1, const char* str2);
char* getEnvpVariable(char* envp[], const char* variable);