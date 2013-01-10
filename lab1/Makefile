# Student's Makefile for the CS:APP Data Lab
CC = gcc
CFLAGS = -O -Wall -m32

btest: btest.c bits.c decl.c tests.c btest.h bits.h
	$(CC) $(CFLAGS) -o btest bits.c btest.c decl.c tests.c

clean:
	rm -f *.o btest

