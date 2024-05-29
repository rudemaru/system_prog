#include "mainfuncs.h"

void *produceMessages() {
    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);       // Открыть семафоры.
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* produceSem = sem_open(PRODUCE_SEM_NAME, 0);
    if(produceSem == SEM_FAILED) {
        printf("Error while opening producing semaphore.\n");
        exit(EXIT_FAILURE);
    }
    while (continuing) {

        sem_wait(queueAccess);                                   // Ожидание доступа.
        sem_wait(produceSem);

        if (queue->countAdded - queue->countDeleted < MAX_MES_COUNT) {  // Добавить, если есть место.
            push(&queue->ringHead, &queue->ringTail);
            queue->countAdded++;
            printf("Added %d message:\n", queue->countAdded);
            printMes(queue->ringTail->message);
        } else
            printf("Queue is full!\n");

        sem_post(produceSem);                                       // Отпустить семафор.
        sem_post(queueAccess);
        sleep(3);
    }

    sem_close(queueAccess);                                      // Отсоединить семафор.
    sem_close(produceSem);
}
void *consumeMessages() {
    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);     // Подключить семафор.
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* consumeSem = sem_open(CONSUME_SEM_NAME, 0);
    if(consumeSem == SEM_FAILED) {
        printf("Error while opening consuming semaphore.\n");
        exit(EXIT_FAILURE);
    }

    while(continuing) {
        sem_wait(queueAccess);                                    // Ожидание доступа.
        sem_wait(consumeSem);

        if(queue->countAdded - queue->countDeleted > 0) {         // Извлечь, если есть что.
            MESSAGE* temp = queue->ringHead->message;
            pop(&queue->ringHead, &queue->ringTail);
            queue->countDeleted++;
            printf("Delete %d message:\n", queue->countDeleted);
            printMes(temp);
            free(temp->data);
            free(temp);
        } else
            printf("Queue is empty!\n");

        sem_post(consumeSem);                                     // Отпустить семафор.
        sem_post(queueAccess);
        sleep(3);
    }

    sem_close(queueAccess);                                       // Отсоединить семафор.
    sem_close(consumeSem);
}

void changeContinuingStatus() {
    continuing ^= 1;
}