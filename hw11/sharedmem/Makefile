# Edit the code below to use different optimization levels. 

CFLAGS0 = 
CFLAGS1 = -O1
CFLAGS2 = -O2
CFLAGS3 = -O3
CFLAGS4 = -O4

# Be sure not to compile timer.c with -O3 or higher
# as it will not work properly!

sharedmem:	timer.o sharedmem.o
	gcc -o sharedmem timer.o sharedmem.o

timer.o: timer.c timer.h
	gcc -c -O2 timer.c       # don't increase above -O2!

sharedmem.o: sharedmem.c timer.h
	gcc -c $(CFLAGS3) sharedmem.c

clean:
	rm *.o shared.dat sharedmem
