#include "mainfuncs.h"

NODE* head = NULL;

int main() {
    signal(SIGINT, printStatistics);
    signal(SIGUSR1, allow);
    signal(SIGUSR2, forbid);
    int keep_alive = 1;
    while(keep_alive) {
        char ch = getchar(), n;
        int num = -1;
        while((n=getchar()) != '\n') {
            {
                if (num == -1){
                    num++;
                }
                num *= 10;
                num += n - '0';
            }
        }
        switch(ch) {
            case '+':
                createChild();
                break;
            case '-':
                deleteLastChild();
                break;
            case 'l':
                printf("Process P with pid = %d\n", getpid());
                print(head);
                break;
            case 'k':
                while(head)
                    deleteLastChild();
                break;
            case 's':
                sendForbid(num);
                break;
            case 'g':
                sendAllow(num);
                break;
            case 'p':
                forbidAndShow(num);
                break;
            case 'q':
                while(head)
                    deleteLastChild();
                printf("Every process killed. Program ended.\n");
                keep_alive = 0;
                break;
            default:
                printf("Invalid argument\n");
                break;
        }
    }

    return 0;
}