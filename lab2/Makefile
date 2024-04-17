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
all: parent child
parent: parent.c parent.h parentfunctions.c Makefile
	$(CC) $(CFLAGS) parent.c parent.h parentfunctions.c -o parent
child: child.c child.h childfunctions.c Makefile
	$(CC) $(CFLAGS) child.c child.h childfunctions.c -o child
run:
	./parent vars.txt
clean:
	rm -f child
	rm -f parent
	rm -rf parent.dSYM
	rm -rf child.dSYM