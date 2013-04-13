CC = gcc
CFLAGS = -O3

all:		tsprocess tsbase tsthread

tsprocess:	tsprocess.c 
		$(CC) $(CFLAGS) tsprocess.c -o tsprocess -lm

tsbase:		tsbase.c
		$(CC) $(CFLAGS) tsbase.c -o tsbase -lm 

tsthread:	tsthread.c
		$(CC) $(CFLAGS) tsthread.c -o tsthread -lm -lpthread

clean:     
	rm tsprocess tsbase tsthread prefix.dat
