#include "serverFunctions.h"

char* defaulDir;
char* helloString;

int main(int argc, char *argv[])
{
    if(argc < 2) {
        printf("Not enought arguments.\n");
        exit(0);
    }
    helloString = readHello(argv[1]);

    if(argc == 3)
        defaulDir = argv[2];
    else
        defaulDir = NULL;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    char *buf;

    int sfds = socket(AF_INET, SOCK_STREAM, 0);
    if (sfds == -1)
    {
        printf("Error while open server.\n");
        close(sfds);
        exit(errno);
    }

    int opt = 1;
    if (setsockopt(sfds, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Error while ser socket options.\n");
        close(sfds);
        exit(errno);
    }

    if (bind(sfds, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        printf("Error while bind server, %d\n", errno);
        close(sfds);
        exit(errno);
    }

    if (listen(sfds, 50) == -1)
    {
        printf("Error while listen server, %d\n", errno);
        close(sfds);
        exit(errno);
    }

    while (1)
    {
        int sfdc = accept(sfds, NULL, 0); // блокирует всё, невозможно ввести ктрл + с
        if (sfdc == -1)
        {
            printf("Error while accept server, %d\n", errno);
            close(sfds);
            close(sfdc);
            exit(errno);
        }

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &sfdc) != 0)
        {
            printf("Error while accept client.\n");
            close(sfds);
            close(sfdc);
            exit(errno);
        }

        pthread_detach(thread);
    }
}