# Edit the code below to use different optimization levels. 

CFLAGS0 = 
CFLAGS1 = -O1
CFLAGS2 = -O2
CFLAGS3 = -O3
CFLAGS4 = -O4

# Be sure not to compile timer.c with -O3 or higher
# as it will not work properly!

getcpe:	timer.o getcpe.o vector.o
	gcc -o getcpe timer.o getcpe.o vector.o

timer.o: timer.c timer.h
	gcc -c -O2 timer.c       # don't increase above -O2!

getcpe.o: getcpe.c timer.h vector.h
	gcc -c $(CFLAGS0) getcpe.c  # edit this line

vector.o: vector.c vector.h
	gcc -c $(CFLAGS0) vector.c  # and this one

clean:
	rm *.o getcpe
