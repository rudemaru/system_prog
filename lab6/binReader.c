#include "structures.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* file = fopen(argv[1], "rb");
    uint64_t count;

    fseek(file, 0, SEEK_END);
    unsigned long amountOfRecords = ftell(file) / sizeof(struct index_s);
    fseek(file, 0, SEEK_SET);

    double temp_1;
    uint64_t temp_2;

    for (int i = 0; i < (int)amountOfRecords; i++) {
        fread(&temp_1, sizeof(double), 1, file);
        fread(&temp_2, sizeof(uint64_t), 1, file);
        printf("%.2ld\t%lf;\t\n", temp_2, temp_1);
    }
    puts("");
}