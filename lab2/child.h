#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

extern char **environ;

#define VAR_NUM 9

char** getFileVariables(const char* fname);                    // Получить список переменных из файла.
int includeString(const char* str1, const char* str2);          // Входит ли строка в другую строку?
char* getEnvpVariable(char* envp[], const char* variable);     // Поиск параметра.