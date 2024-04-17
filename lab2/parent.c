#include "parent.h"

int main(int argc, char* argv[], char* envp[]) {

    setlocale(LC_COLLATE, "C");

    if(argc < 2){
        printf("Error: not enough arguments.\n");
        exit(1);
    }

    int i = 0;
    while (envp[i++]);
    qsort(envp, i-1, sizeof(char*), compare);
    i = 0;
    while(envp[i]) {
        printf("%s\n", envp[i++]);
    }

    char *childName = (char*)calloc(9, sizeof(char));
    strcpy(childName, "child_00");

    i = 0;

    while(1) {
        char key = getchar();
        while (getchar() != '\n')
            continue;
        if (key == 'q')
            break;
        else{
            pid_t chpid = fork();
            if (chpid == 0) {
                char *childDir = NULL;
                switch (key) {
                    case '+':
                        childDir = getenv("CHILD_PATH");
                        break;
                    case '*':
                        childDir = scanEnvp(envp);
                        break;
                    case '&':
                        childDir = scanEnvp(environ);
                        break;
                    default:
                        printf("Unknown option.\nAvailable options:\n+\t*\t&\tq\n");
                        break;
                }
                if (childDir) {
                    char *newArgv[] = {childName, argv[1], &key, NULL};
                    execve(childDir, newArgv, envp);
                } else if (key == '+' || key == '*' || key == '&')
                    printf("Error: path not found.\n");

            } else if (chpid == -1) {
                printf("Error: error code - %d\n", errno);
                exit(errno);
            } else {
                i++;
                setName(&childName, i);
            }
        }
        waitpid(-1, NULL, WNOHANG);
    }

    free(childName);

    return 0;
}