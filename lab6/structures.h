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

#define JULIAN_DAY_START 15020.0

typedef struct index_s {
    double time_mark;
    uint64_t recno;
} Record;
