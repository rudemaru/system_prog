CC = gcc
CFLAGS_DEBUG = -g -ggdb -std=c11 -pedantic -W -Wall -Wextra
CFLAGS_RELEASE = -std=c11 -pedantic -W -Wall -Wextra
.PHONY: clean all run
MODE ?= debug
ifeq ($(MODE), debug)
	CFLAGS = $(CFLAGS_DEBUG)
else
	CFLAGS = $(CFLAGS_RELEASE)
endif
all: program
program: main.c mainfuncs.c stack.c Makefile
	$(CC) -o program *.c
run:
	./program
clean:
	rm -f program
	rm -rf program.dSYM