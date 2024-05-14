#include "child.h"

char** getFileVariables(const char* fileName) {
    FILE *f = NULL;
    if ((f = fopen(fileName, "r")) != NULL) {
        char **result = (char **)calloc(VAR_NUM, sizeof(char *));
        for (int i = 0; i < VAR_NUM; i++) {
            result[i] = (char*)calloc(80, sizeof(char));
            fgets(result[i], 80, f);
            result[i][strlen(result[i])-1] = '\0';
        }
        fclose(f);
        return result;
    }
    printf("Error: can't open file\n");
    return NULL;
}

int includeString(const char* str1, const char* str2) {
    for(size_t i = 0; i<strlen(str2)-1; i++)
        if(str1[i]!=str2[i])
            return 0;
    return 1;
}

char* getEnvpVariable(char* envp[], const char* variable) {
    int i = 0;
    while(envp[i]) {
        if(includeString(envp[i], variable))
            break;
        i++;
    }
    return envp[i];
}