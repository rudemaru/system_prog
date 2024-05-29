#include "mainfuncs.h"

void sendingMessages() {
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);
    if(shm_fd == -1) {
        printf("Error while opening shared memory.\n");
        exit(EXIT_FAILURE);
    }

    QUEUE* queue = mmap(NULL, sizeof(QUEUE)+MAX_MES_COUNT*sizeof(NODE)+MAX_MES_COUNT*sizeof(MESSAGE), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(queue == MAP_FAILED) {
        printf("Error while mapping shared memory.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* sendSem = sem_open(SEND_SEM_NAME, 0);
    if(sendSem == SEM_FAILED) {
        printf("Error while opening filling semaphore.\n");
        exit(EXIT_FAILURE);
    }

    while (continuing) {
        sem_wait(queueAccess);
        sem_wait(sendSem);

        if (queue->countAdded - queue->countDeleted < MAX_MES_COUNT) {    // Добавить, если есть место.
            push(queue);
            queue->countAdded++;
            printf("Added %d message:\n\n", queue->countAdded);
            printMes((MESSAGE*)(((NODE*)(queue->ringTail + (uintptr_t)queue))->message));
        } else
            printf("Queue is full!\n\n");

        sem_post(sendSem);
        sem_post(queueAccess);
        sleep(5);
    }
                                                                 // Отсоединить память.
    munmap(queue, sizeof(QUEUE)+MAX_MES_COUNT*sizeof(NODE)+MAX_MES_COUNT*sizeof(MESSAGE));
    close(shm_fd);

    sem_close(queueAccess);
    sem_close(sendSem);
}
void receivingMessages() {
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);     // Открыть совместо используемую память.
    if(shm_fd == -1) {
        printf("Error while opening shared memory. Code: %d\n", errno);
        exit(EXIT_FAILURE);
    }
                                                                 // Отобразить совместо используемую память.
    QUEUE* queue = mmap(NULL, sizeof(QUEUE)+MAX_MES_COUNT*sizeof(NODE)+MAX_MES_COUNT*sizeof(MESSAGE), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(queue == MAP_FAILED) {
        printf("Error while mapping shared memory.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* receiveSem = sem_open(RECEIVE_SEM_NAME, 0);
    if(receiveSem == SEM_FAILED) {
        printf("Error while opening extracting semaphore.\n");
        exit(EXIT_FAILURE);
    }

    while(continuing) {
        sem_wait(queueAccess);
        sem_wait(receiveSem);

        if(queue->countAdded - queue->countDeleted > 0) {
            printf("Delete %d message:\n\n", queue->countDeleted+1);
            printMes((MESSAGE*)(((NODE*)(queue->ringHead + (uintptr_t)queue))->message));
            pop(queue);
            queue->countDeleted++;
        } else
            printf("Queue is empty!\n\n");

        sem_post(receiveSem);
        sem_post(queueAccess);
        sleep(5);
    }
                                                                 // Отсоединить память.
    munmap(queue, sizeof(QUEUE)+MAX_MES_COUNT*sizeof(NODE)+MAX_MES_COUNT*sizeof(MESSAGE));
    close(shm_fd);

    sem_close(queueAccess);
    sem_close(receiveSem);
}

void changeContinuingStatus() {
    continuing ^= 1;
}