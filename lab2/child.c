#include "child.h"

int main(int argc, char* argv[], char* envp[]) {

    if(argc < 2){
        printf("Error: not enough arguments passed to child.\n");
        exit(1);
    }

    printf("Process name: %s\nProcess pid: %d\nProcess ppdid: %d\n", argv[0], (int) getpid(), (int) getppid());

    //Чтение названий переменных среды из переданного файла
    char **variables = getFileVariables(argv[1]);
    if (variables != NULL) {
        //Получение значений переменных среды
        switch (argv[2][0]) {
            case '+':
                for (int i = 0; i < VAR_NUM; i++)
                    //Поиск среди переменных среды из getenv
                    printf("%s=%s\n", variables[i], getenv(variables[i]));
                break;
            case '*':
                for (int i = 0; i < VAR_NUM; i++) {
                    //Поиск по названию в envp
                    char* response = getEnvpVariable(envp, variables[i]); 
                    if(response != NULL)
                        printf("%s\n", response);
                }
                break;
            case '&':
                for (int i = 0; i < VAR_NUM; i++) {
                    //Поиск по названию в environ
                    char *response = getEnvpVariable(environ, variables[i]);
                    if (response != NULL)
                        printf("%s\n", response);
                }
                break;
        }
        for (int i = 0; i < VAR_NUM; i++)
            free(variables[i]);
    }
    free(variables);
    return 0;
}