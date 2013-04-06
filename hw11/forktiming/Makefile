# Edit the code below to use different optimization levels. 

CFLAGS0 = 
CFLAGS1 = -O1
CFLAGS2 = -O2
CFLAGS3 = -O3
CFLAGS4 = -O4

# Be sure not to compile timer.c with -O3 or higher
# as it will not work properly!

forktiming:	timer.o forktiming.o
	gcc -o forktiming timer.o forktiming.o

timer.o: timer.c timer.h
	gcc -c -O2 timer.c       # don't increase above -O2!

forktiming.o: forktiming.c timer.h
	gcc -c $(CFLAGS3) forktiming.c

clean:
	rm *.o forktiming
