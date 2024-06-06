#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <netdb.h>

char *currentDir;
int chDir;
int sfdc;

int doOperation(char **buf)
{
    if (strstr(*buf, "CD") != NULL)
        chDir = 1;
    write(sfdc, *buf, strlen(*buf) + 1);
    free(*buf);
    *buf = (char *)malloc(255);
    read(sfdc, *buf, 255);
    if (chDir == 1)
    {
        if (strcmp(*buf, "NULL") != 0)
        {
            free(currentDir);
            currentDir = (char *)malloc(strlen(*buf) + 1);
            for (int i = 0; i < strlen(*buf); i++)
                currentDir[i] = (*buf)[i];
            currentDir[strlen(*buf)] = '\0';
        }
        else
        {
            free(currentDir);
            currentDir = (char *)malloc(1);
            currentDir[0] = '\0';
        }
        free(*buf);
        return 1;
    }
    if (strcmp(*buf, "NULL") != 0)
        printf("%s\n", *buf);
    if (strcmp(*buf, "BYE") == 0)
    {
        free(*buf);
        return 0;
    }
    free(*buf);
    return 1;
}

int main(int argc, char *argv[])
{
    if(argc<2) {
        printf("Not enought arguments.\n");
        exit(0);
    }


    struct hostent* server;
    struct sockaddr_in addr;

    server = gethostbyname(argv[1]);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    bcopy((char*)server->h_addr_list[0], (char*)&addr.sin_addr.s_addr, server->h_length);
    addr.sin_addr.s_addr = INADDR_ANY;

    currentDir = (char *)malloc(1);
    currentDir[0] = '\0';

    sfdc = socket(AF_INET, SOCK_STREAM, 0);
    if (sfdc == -1)
    {
        printf("Error while open client.\n");
        close(sfdc);
        exit(errno);
    }

    while (connect(sfdc, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        if (errno == ENOENT)
        {
            sleep(1);
            continue;
        }
        else
        {
            printf("Error while connect client.\n");
            close(sfdc);
            exit(errno);
        }
    }

    char* firstMessage = (char*)malloc(255);
    read(sfdc, firstMessage, 255);
    printf("%s\n", firstMessage);
    free(firstMessage);

    int continuing = 1;
    while (continuing)
    {
        chDir = 0;
        printf("%s>", currentDir);
        char *buf = (char *)malloc(255);
        fgets(buf, 255, stdin);
        buf[strlen(buf) - 1] = '\0';
        if (strstr(buf, "@") != NULL)
        {
            char *fname = (char *)malloc(strlen(buf));
            for (int i = 0; i < strlen(buf); i++)
                fname[i] = buf[i + 1];
            fname[strlen(buf) - 1] = '\0';

            FILE *f = fopen(fname, "r");
            if (f == NULL)
            {
                printf("Error open file.\n");
                continue;
            }
            free(buf);
            while (!feof(f))
            {
                buf = (char *)malloc(255);
                fgets(buf, 255, f);
                buf[strlen(buf) - 1] = '\0';
                if (strlen(buf) > 1)
                {
                    printf("%s>%s\n", currentDir, buf);
                    if (!doOperation(&buf)) {
                        continuing = 0;
                        break;
                    }
                }
            }
            fclose(f);
        }
        else
        {
            if (!doOperation(&buf))
                continuing = 0;;
        }
    }

    close(sfdc);
    exit(0);
}