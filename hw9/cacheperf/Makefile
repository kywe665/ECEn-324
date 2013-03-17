# Edit the code below to use different optimization levels. 

CFLAGS0 = -O3
CFLAGS1 = -O1
CFLAGS2 = -O2
CFLAGS3 = -O3
CFLAGS4 = -O4

# Be sure not to compile timer.c with -O3 or higher
# as it will not work properly!

cacheperf:	timer.o cacheperf.o
	gcc -o cacheperf timer.o cacheperf.o

timer.o: timer.c timer.h
	gcc -c -O2 timer.c       # don't increase above -O2!

cacheperf.o: cacheperf.c timer.h
	gcc -c $(CFLAGS0) cacheperf.c

clean:
	rm *.o cacheperf
