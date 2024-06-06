CC=gcc
CFLAGS = -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic -pthread

SRCS_FILEGENERATOR=fileGenerator.c
SRCS_FILEMAIN=main.c

OBJS_FILEGENERATOR=$(SRCS_FILEGENERATOR:.c=.o)
OBJS_FILEMAIN=$(SRCS_FILEMAIN:.c=.o)

all: fileGenerator main

fileGenerator: $(OBJS_FILEGENERATOR)
	$(CC) $(CFLAGS) $(OBJS_FILEGENERATOR) -o $@

fileSorter: $(OBJS_FILESORTER)
	$(CC) $(CFLAGS) $(OBJS_FILESORTER) -o $@

clean:
	rm -f $(OBJS_FILEGENERATOR) $(OBJS_FILESORTER) fileGenerator main