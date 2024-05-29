#include "mainfuncs.h"

int continuing = 1;

STACKNODE* stackSender = NULL;
STACKNODE* stackReceiver = NULL;
 
int main() {
    srand(time(NULL));

    shm_unlink(SHARED_MEMORY_NAME);
    sem_unlink(SEND_SEM_NAME);
    sem_unlink(RECEIVE_SEM_NAME);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    signal(SIGUSR1, changeContinuingStatus);

    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1) {
        printf("Error while creating shared memory.\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(shm_fd, sizeof(QUEUE)+MAX_MES_COUNT*sizeof(NODE)+MAX_MES_COUNT*sizeof(MESSAGE));

    QUEUE* queue = mmap(NULL, sizeof(QUEUE)+MAX_MES_COUNT*sizeof(NODE)+MAX_MES_COUNT*sizeof(MESSAGE), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(queue == MAP_FAILED) {
        printf("Error while mapping shared data.\n");
        exit(EXIT_FAILURE);
    }
    queue->ringHead = 0;
    queue->ringTail = 0;
    queue->countDeleted = 0;
    queue->countAdded = 0;
    queue->currentPlaceToWrite = (uintptr_t)queue + sizeof(QUEUE);

    sem_t* sendSem;
    if((sendSem = sem_open(SEND_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open filling semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t* receiveSem;
    if((receiveSem = sem_open(RECEIVE_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open receiveing semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t* queueAccess;
    if((queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open queue semaphore, code %d.\n", errno);
        exit(errno);
    }

    while(continuing) {
        char ch = getchar();
        switch (ch) {
            case 's': { //create sender
                pid_t pid = fork();
                if(pid == -1) {
                    printf("Error occurred while creating new sender, error code %d.\n", errno);
                    exit(errno);
                } else if(pid == 0) {
                    sendingMessages();
                    exit(0);
                } else
                    pushStack(&stackSender, pid);
                break;
            }
            case 'k': //delete sender
                if(stackSender!=NULL) {
                    kill(stackSender->pid, SIGUSR1);
                    waitpid(stackSender->pid, NULL, 0);
                    popStack(&stackSender);
                } else
                    printf("There is no senders.\n");
                break;
            case 'r': { //create receiver
                pid_t pid = fork();
                if(pid == -1) {
                    printf("Error occurred while creating new receiver, error code %d.\n", errno);
                    exit(errno);
                } else if(pid == 0) {
                    receivingMessages();
                    exit(0);
                } else
                    pushStack(&stackReceiver, pid);
                break;
            }
            case 'l': // delete receiver
                if(stackReceiver!=NULL) {
                    kill(stackReceiver->pid, SIGUSR1);
                    waitpid(stackReceiver->pid, NULL, 0);
                    popStack(&stackReceiver);
                } else
                    printf("There is no receivers.\n");
                break;
            case 'q':
                while(stackSender != NULL) {                              // Конец.
                    kill(stackSender->pid, SIGUSR1);
                    waitpid(stackSender->pid, NULL, 0);
                    popStack(&stackSender);
                }
                while(stackReceiver != NULL) {
                    kill(stackReceiver->pid, SIGUSR1);
                    waitpid(stackReceiver->pid, NULL, 0);
                    popStack(&stackReceiver);
                }
                printf("All senders are receivers are killed. End of program.\n");
                continuing = 0;
                break;
        }
    }

    munmap(queue, sizeof(QUEUE)+MAX_MES_COUNT*sizeof(NODE)+MAX_MES_COUNT*sizeof(MESSAGE));
    close(shm_fd);
    shm_unlink(SHARED_MEMORY_NAME);

    sem_close(sendSem);
    sem_unlink(SEND_SEM_NAME);
    sem_close(receiveSem);
    sem_unlink(RECEIVE_SEM_NAME);
    sem_close(queueAccess);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    return 0;
}