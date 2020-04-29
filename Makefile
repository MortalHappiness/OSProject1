.PHONY: clean
CC = gcc
CFLAGS = -c

all: project1
	@echo -n ""

project1: main.o scheduler.o process.o
	$(CC) -o $@ $+

%.o: %.c common.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o project1

