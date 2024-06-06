#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FILENAME "data.bin"

struct Record {
    char name[80];
    char address[80];
    uint8_t semester;
};

void create_sample_records(struct Record records[], int count) {
    for (int i = 0; i < count; i++) {
        snprintf(records[i].name, sizeof(records[i].name), "Student %d", i + 1);
        snprintf(records[i].address, sizeof(records[i].address), "Address %d", i + 1);
        records[i].semester = (i % 8) + 1;
    }
}

int main() {
    struct Record records[10];
    create_sample_records(records, 10);

    FILE *file = fopen(FILENAME, "wb");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    size_t written = fwrite(records, sizeof(struct Record), 10, file);
    if (written != 10) {
        perror("fwrite");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    printf("File '%s' created with 10 records.\n", FILENAME);
    return 0;
}