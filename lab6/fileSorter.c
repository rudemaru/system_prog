#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "structures.h"

int blocks;
int threads;
size_t blockSize;
char *filename;

pthread_barrier_t barrier;
pthread_mutex_t mutex;
Record *fileMemoryAddress;
int *blocksStatusMap;

void print() {
    for (int i = 0; i < (int)(blockSize * blocks); i++) {
        printf("%ld\t%lf\t\n", fileMemoryAddress[i].recno, fileMemoryAddress[i].time_mark);
    }
    puts("\n");
}

void printRecordsFromFile(const char* fileName) {
    FILE* file = fopen(fileName, "rb");
    fseek(file, 0, SEEK_END);
    unsigned long amountOfRecords = ftell(file) / sizeof(Record);
    fseek(file, 0, SEEK_SET);

    Record temp_record;

    for (int i = 0; i < (int)amountOfRecords; i++) {
        fread(&temp_record, sizeof(Record), 1, file);
        printf("%lu\t%lf;\n", temp_record.recno, temp_record.time_mark);
        if (!((i + 1) % 8)) {
            puts("");
        }
        if (!((i + 1) % 256)) {
            puts("");
        }
    }
    puts("");
}

int isSubOfTwo(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

int check() { // функция check без аргументов
    if (!isSubOfTwo(blocks) || blocks > 256) {
        return -2;
    } if (threads > 64 || threads < 4 || blocks < threads) {
        return -4;
    }
    return 1;
}

int compareBlocks(const void* a, const void* b) {
    Record num1 = *((Record*)a);
    Record num2 = *((Record*)b);
    if (num1.time_mark < num2.time_mark)
        return -1;
    else if (num1.time_mark > num2.time_mark)
        return 1;
    else
        return 0;
}

int selectNextBlock(int iteration) {
    int number = -1;

    pthread_mutex_lock(&mutex);

    for (int i = 0; i < blocks / iteration; i++) {
        if (!blocksStatusMap[i]) {
            blocksStatusMap[i] = 1;
            number = i;
            break; // Переключаемся на новый блок сразу после его выбора
        }
    }

    pthread_mutex_unlock(&mutex);

    return number;
}


void *sort(void *arg) {
    int argument = *(int*)arg;
    int number = argument;
    int iteration = 1;

    pthread_barrier_wait(&barrier);

    while (number >= 0) {
        Record* block = (Record*)malloc(blockSize * sizeof(Record));
        for (int i = 0; i < blockSize; i++) {
            block[i] = fileMemoryAddress[blockSize * number + i];
        }

        qsort(block, blockSize, sizeof(Record), compareBlocks);

        for (int i = 0; i < blockSize; i++) {
            fileMemoryAddress[blockSize * number + i] = block[i];
        }

        number = selectNextBlock(1);
        if (number == -1) {
            break;
        }
        free(block);
    }

    size_t newBlockSize = blockSize;
    pthread_barrier_wait(&barrier);
    for (int i = 0; i < blocks; i++) {
        blocksStatusMap[i] = 0;
    }

    while (iteration != blocks) {
        number = argument;
        iteration *= 2;
        newBlockSize *= 2;

        pthread_barrier_wait(&barrier);

        while (number >= 0) {
            number = selectNextBlock(iteration);
            if (number == -1) {
                break;
            }
            Record* block = (Record*)calloc(newBlockSize, sizeof(Record));
            for (int i = 0; i < newBlockSize; i++) {
                block[i] = fileMemoryAddress[i + newBlockSize * number];
            }

            int i = 0;
            int j = newBlockSize / 2;
            int k = 0;
            for (; (i < newBlockSize / 2) && (j < newBlockSize);) {
                if (block[i].time_mark > block[j].time_mark) {
                    fileMemoryAddress[k + newBlockSize * number] = block[j];
                    j++;
                } else {
                    fileMemoryAddress[k + newBlockSize * number] = block[i];
                    i++;
                }
                k++;
            }

            while (j < newBlockSize) {
                fileMemoryAddress[k + newBlockSize * number] = block[j++];
                k++;
            }
            while (i < newBlockSize / 2) {
                fileMemoryAddress[k + newBlockSize * number] = block[i++];
                k++;
            }

            free(block);
        }

        pthread_barrier_wait(&barrier);

        for (int i = 0; i < blocks / iteration; i++) {
            blocksStatusMap[i] = 0;
        }
    }

    pthread_barrier_wait(&barrier);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        puts("./имя_программы <количество блоков> <количество потоков> <имя файла>");
        return 1;
    }
    blocks   = atoi(argv[1]);
    threads  = atoi(argv[2]);
    filename = strdup(argv[3]);

    int status = check();
    if (status == -2) {
        puts("blocks");
        puts("Количество блоков не должно быть больше 256");
        free(filename);
        return 0;
    } if (status == -4) {
        puts("threads");
        puts("Количество потоков должно лежать в пределах от 4 до 64 и быть не больше, чем количество блоков");
        free(filename);
        return 0;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, threads);

    int file = open(filename, O_RDWR);
    if (file < 0) {
        perror("Ошибка открытия файла");
        free(filename);
        return EXIT_FAILURE;
    }
    size_t fileSize = lseek(file, 0, SEEK_END);
    lseek(file, 0, SEEK_SET);

    fileMemoryAddress = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    if (fileMemoryAddress == MAP_FAILED) {
        perror("Ошибка мапинга файла в память");
        free(filename);
        return EXIT_FAILURE;
    }

    blockSize = fileSize / blocks / sizeof(Record);

    blocksStatusMap = (int *)calloc(blocks, sizeof(int));
    for (int i = 0; i < threads; i++) {
        blocksStatusMap[i] = 1;
    }
    for (int i = threads; i < blocks; i++) {
        blocksStatusMap[i] = 0;
    }

    pthread_t *threadsArray = calloc(threads, sizeof(pthread_t));
    int *threadsIds = calloc(threads, sizeof(int));
    for (int i = 0; i < threads; i++) {
        threadsIds[i] = i;
        (void) pthread_create(&threadsArray[i], NULL, sort, &threadsIds[i]);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(threadsArray[i], NULL);
    }

    if (msync(fileMemoryAddress, fileSize, MS_SYNC) == -1) {
        perror("Ошибка синхронизации памяти с файлом");
        munmap(fileMemoryAddress, fileSize);
        free(filename);
        free(threadsArray);
        free(threadsIds);
        close(file);
        return EXIT_FAILURE;
    }
    print();
    free(threadsIds);
    free(threadsArray);
    munmap(fileMemoryAddress, fileSize);
    close(file);
    free(filename);
    return EXIT_SUCCESS;
}