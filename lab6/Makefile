CC=gcc
CFLAGS=-Wall -Wextra -pthread -O2 -D_POSIX_C_SOURCE=200112L

SRCS_FILEGENERATOR=fileGenerator.c
SRCS_FILESORTER=fileSorter.c
SRCS_BINREADER=binReader.c

OBJS_FILEGENERATOR=$(SRCS_FILEGENERATOR:.c=.o)
OBJS_FILESORTER=$(SRCS_FILESORTER:.c=.o)
OBJS_BINREADER=$(SRCS_BINREADER:.c=.o)

all: fileGenerator fileSorter binReader

fileGenerator: $(OBJS_FILEGENERATOR)
	$(CC) $(CFLAGS) $(OBJS_FILEGENERATOR) -o $@

fileSorter: $(OBJS_FILESORTER)
	$(CC) $(CFLAGS) $(OBJS_FILESORTER) -o $@

binReader: $(OBJS_BINREADER)
	$(CC) $(CFLAGS) $(OBJS_BINREADER) -o $@

clean:
	rm -f $(OBJS_FILEGENERATOR) $(OBJS_FILESORTER) fileGenerator fileSorter binReader