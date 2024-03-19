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
all: dirwalk
dirwalk: dirwalk.c Makefile
	$(CC) $(CFLAGS) dirwalk.c -o dirwalk
clean:
	rm -f dirwalk
	rm -rf dirwalk.dSYM