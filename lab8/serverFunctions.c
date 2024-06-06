#include "serverFunctions.h"

void changeDirectory(NODE **stackDir, char *buf)
{
    DIR *dir = opendir(plusString(defaulDir, topStr(*stackDir)));
    if (!dir)
    {
        perror("opendir");
        exit(errno);
    }
    struct dirent *curdir;
    for (;;)
    {
        curdir = readdir(dir);
        if (curdir == NULL)
        {
            if (errno)
            {
                printf("Error while reading directory.\n");
                return;
            }
            break;
        }
        if (strstr(buf, curdir->d_name) != NULL)
        {
            if (strlen(buf) > 3 && strstr(buf, "..") == NULL)
            {
                char *subDir = (char *)malloc(strlen(buf) - 2);
                for (int i = 0; i < strlen(buf) - 3; i++)
                    subDir[i] = buf[i + 3];
                subDir[strlen(buf) - 3] = '\0';
                char *newDir = (char *)malloc(strlen(topStr(*stackDir)) + 2 + strlen(subDir));
                int i = 0;
                for (int j = 0; j < strlen(topStr(*stackDir)); j++)
                    newDir[i++] = topStr(*stackDir)[j];
                newDir[i++] = '/';
                for (int j = 0; j < strlen(subDir); j++)
                    newDir[i++] = subDir[j];
                newDir[i] = '\0';
                push(stackDir, newDir);
                free(subDir);
            }
            break;
        }
    }
    closedir(dir);
}

void listDirectory(NODE *stackDir, char **result)
{
    DIR *dir = opendir(plusString(defaulDir, topStr(stackDir)));
    if (!dir)
    {
        perror("opendir");
        exit(errno);
    }

    struct dirent *curdir;
    for (;;)
    {
        curdir = readdir(dir);
        if (curdir == NULL)
        {
            if (errno)
            {
                printf("Error while reading directory.\n");
                return;
            }
            break;
        }

        switch (curdir->d_type)
        {
            case DT_DIR:
            {
                if (strcmp(curdir->d_name, ".") != 0 && strcmp(curdir->d_name, "..") != 0)
                {
                    int i = strlen(*result);
                    *result = (char *)realloc(*result, strlen(*result) + strlen(curdir->d_name) + 3);
                    for (int j = 0; j < strlen(curdir->d_name); j++)
                        (*result)[i++] = curdir->d_name[j];
                    (*result)[i++] = '/';
                    (*result)[i++] = '\n';
                    (*result)[i] = '\0';
                }
                break;
            }
            case DT_LNK:
            {
                int i = strlen(*result);
                *result = (char *)realloc(*result, strlen(*result) + strlen(curdir->d_name) + 5);
                for (int j = 0; j < strlen(curdir->d_name); j++)
                    (*result)[i++] = curdir->d_name[j];
                (*result)[i++] = ' ';
                (*result)[i++] = '-';
                (*result)[i++] = '-';
                (*result)[i++] = '>';
                (*result)[i] = '\0';
                char linkTarget[80];
                if (readlink(plusString(plusString(defaulDir, topStr(stackDir)), curdir->d_name), linkTarget, sizeof(linkTarget) - 1) != -1)
                {
                    struct stat targetStat;
                    if (lstat(linkTarget, &targetStat) == -1)
                    {
                        printf("error lstat.\n");
                        break;
                    }
                    if (S_ISREG(targetStat.st_mode))
                    {
                        *result = (char *)realloc(*result, strlen(*result) + strlen(linkTarget) + 3);
                        (*result)[i++] = ' ';
                        for (int j = 0; j < strlen(linkTarget); j++)
                            (*result)[i++] = linkTarget[j];
                        (*result)[i++] = '\n';
                        (*result)[i] = '\0';
                    }
                    else if (S_ISLNK(targetStat.st_mode))
                    {
                        *result = (char *)realloc(*result, strlen(*result) + strlen(linkTarget) + 4);
                        (*result)[i++] = '>';
                        (*result)[i++] = ' ';
                        for (int j = 0; j < strlen(linkTarget); j++)
                            (*result)[i++] = linkTarget[j];
                        (*result)[i++] = '\n';
                        (*result)[i] = '\0';
                    }
                }
                break;
            }
            case DT_REG:
            {
                int i = strlen(*result);
                *result = (char *)realloc(*result, strlen(*result) + strlen(curdir->d_name) + 2);
                for (int j = 0; j < strlen(curdir->d_name); j++)
                    (*result)[i++] = curdir->d_name[j];
                (*result)[i++] = '\n';
                (*result)[i++] = '\0';
                break;
            }
        }
    }
    closedir(dir);
}

void *handle_client(void *arg)
{
    NODE *stackDir = NULL;
    char *temp = (char *)malloc(2);
    temp[0] = '.';
    temp[1] = '\0';
    push(&stackDir, temp);

    int sfdc = *((int *)arg);

    write(sfdc, SERVER_MESSAGE, strlen(SERVER_MESSAGE) + 1);

    while (1)
    {
        char *buf = (char *)malloc(80);
        read(sfdc, buf, 80);

        if (strstr(buf, "QUIT") != NULL)
        {
            write(sfdc, "BYE", 4);
            free(buf);
            break;
        }
        else if (strstr(buf, "ECHO") != NULL)
        {
            char *result = (char *)malloc(strlen(buf) - 4);
            for (int i = 0; i < strlen(buf) - 5; i++)
                result[i] = buf[i + 5];
            result[strlen(buf) - 5] = '\0';
            write(sfdc, result, strlen(result) + 1);
            free(result);
        }
        else if (strstr(buf, "INFO") != NULL)
        {
            write(sfdc, helloString, strlen(helloString) + 1);
        }
        else if (strstr(buf, "CD") != NULL)
        {
            if (strstr(buf, "..") != NULL && strcmp(topStr(stackDir), ".") != 0)
                pop(&stackDir);
            else
                changeDirectory(&stackDir, buf);
            if (strcmp(topStr(stackDir), ".") != 0)
            {
                char *response = (char *)malloc(strlen(topStr(stackDir)) - 1);
                for (int i = 0; i < strlen(topStr(stackDir)); i++)
                    response[i] = topStr(stackDir)[i + 2];
                response[strlen(topStr(stackDir)) - 2] = '\0';
                write(sfdc, response, strlen(response) + 1);
                free(response);
            }
            else
                write(sfdc, "NULL", strlen("NULL") + 1);
        }
        else if (strstr(buf, "LIST") != NULL)
        {
            char *result = (char *)malloc(1);
            result[0] = '\0';

            listDirectory(stackDir, &result);

            result[strlen(result) - 1] = '\0';
            write(sfdc, result, strlen(result) + 1);
            free(result);
        }
        else
            write(sfdc, "NULL", strlen("NULL") + 1);
        free(buf);
    }

    while (stackDir)
        pop(&stackDir);

    close(sfdc);
    pthread_exit(NULL);
}

char* readHello(char* fname) {
    char* temp;

    FILE* f = fopen(fname, "r");
    if(f == NULL) {
        printf("Error while open file.\n");
        exit(0);
    }

    temp = (char*)malloc(255);

    fgets(temp, 255, f);

    fclose(f);

    temp[strlen(temp) - 1] = '\0';
    return temp;
}

char* plusString(char* str1, char* str2) {
    if(str1 != NULL) {
        char* result = (char*)malloc(strlen(str1) + strlen(str2) - 1 + 2);

        int i = 0;
        for(int j = 0; j<strlen(str1); j++)
            result[i++] = str1[j];

        for(int j = 1; j<strlen(str2); j++)
            result[i++] = str2[j];

        result[i] = '\0';

        return result;
    }
    else
        return str2;
}