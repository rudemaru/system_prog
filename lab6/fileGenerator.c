#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "structures.h"

void printRecordsFromFile(const char* fileName) {
    FILE* file = fopen(fileName, "rb");
    uint64_t count;

    fseek(file, 0, SEEK_END);
    unsigned long amountOfRecords = ftell(file) / sizeof(struct index_s);
    fseek(file, 0, SEEK_SET);

    double temp_1;
    uint64_t temp_2;

    for (int i = 0; i < (int)amountOfRecords; i++) {
        fread(&temp_1, sizeof(double), 1, file);
        fread(&temp_2, sizeof(uint64_t), 1, file);
        printf("%.2ld\t%lf;\t", temp_2, temp_1);
    }
    puts("");
}

double random_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}


double generate_time_mark(double max_julian_date) {
    double integerDayPart = JULIAN_DAY_START + rand() % ((int)max_julian_date - (int)JULIAN_DAY_START);
    double fractional_day = random_double(0.0, 0.999999);
    return integerDayPart + fractional_day;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование с параметрами: %s <amountOfRecords> <filename>", argv[0]);
        return EXIT_FAILURE;
    }

    size_t amountOfRecords = atoi(argv[1]);
    amountOfRecords = ((amountOfRecords + 255) / 256) * 256;
    char *filename = argv[2];

    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }


    srand(getpid());
    for (size_t i = 0; i < amountOfRecords; i++) {
        struct index_s record = {
                .time_mark = generate_time_mark(60435),
                .recno = i + 1
        };
        printf("\n%.2ld\t%lf\n", record.recno, record.time_mark);
        fwrite(&record, sizeof(struct index_s), 1, file);
    }


    fclose(file);

    return EXIT_SUCCESS;
}