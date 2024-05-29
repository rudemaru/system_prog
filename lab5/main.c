#include "mainfuncs.h"

int continuing = 1;
int MAX_MES_COUNT = 20;

STACKNODE* stackProducer = NULL;
STACKNODE* stackConsumer = NULL;

QUEUE* queue;

int main() {
    srand(time(NULL));

    sem_unlink(PRODUCE_SEM_NAME);
    sem_unlink(CONSUME_SEM_NAME);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    signal(SIGUSR1, changeContinuingStatus);

    queue = (QUEUE*)malloc(sizeof(QUEUE));
    queue->ringHead = 0;
    queue->ringTail = 0;
    queue->countDeleted = 0;
    queue->countAdded = 0;

    sem_t* produceSem;
    if((produceSem = sem_open(PRODUCE_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error opening producer semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t* consumeSem;
    if((consumeSem = sem_open(CONSUME_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error opening consumer semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t* queueAccess;
    if((queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error opening queue semaphore, code %d.\n", errno);
        exit(errno);
    }

    while(continuing) {
        char ch = getchar();
        switch (ch) {
            case 'p': {
                pthread_t threadId;
                if(pthread_create(&threadId, NULL, produceMessages, NULL) != 0) {
                    printf("Error while creating producer thread.");
                    continuing = 0;
                } else
                    pushStack(&stackProducer, threadId);
                break;
            }
            case 's':
                if(stackProducer!=NULL) {
                    pthread_cancel(stackProducer->threadId);
                    pthread_join(stackProducer->threadId, NULL);
                    popStack(&stackProducer);
                    continuing = 1;
                } else
                    printf("There are no producers.\n");
                break;
            case 'c': {
                pthread_t threadId;
                if(pthread_create(&threadId, NULL, consumeMessages, NULL) != 0) {
                    printf("Error while creating consumer thread.");
                    continuing = 0;
                } else
                    pushStack(&stackConsumer, threadId);
                break;
            }
            case 'd':
                if(stackConsumer!=NULL) {
                    pthread_cancel(stackConsumer->threadId);
                    pthread_join(stackConsumer->threadId, NULL);
                    popStack(&stackConsumer);
                    continuing = 1;
                } else
                    printf("There are no consumers.\n");
                break;
            case '+':
                MAX_MES_COUNT++;
                break;
            case '-':
                if(queue->countAdded - queue->countDeleted < MAX_MES_COUNT)
                    MAX_MES_COUNT--;
                break;
            case 'q':
                while(stackProducer != NULL) {
                    pthread_cancel(stackProducer->threadId);
                    pthread_join(stackProducer->threadId, NULL);
                    popStack(&stackProducer);
                    continuing = 1;
                    printf("Producer killed!\n");
                }
                while(stackConsumer != NULL) {
                    pthread_cancel(stackConsumer->threadId);
                    pthread_join(stackConsumer->threadId, NULL);
                    popStack(&stackConsumer);
                    continuing = 1;
                    printf("Consumer killed!\n");
                }
                printf("All producers and consumers are killed. End of program.\n");
                continuing = 0;
                break;
        }
    }

    sem_close(produceSem);
    sem_unlink(PRODUCE_SEM_NAME);
    sem_close(consumeSem);
    sem_unlink(CONSUME_SEM_NAME);
    sem_close(queueAccess);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    return 0;
}