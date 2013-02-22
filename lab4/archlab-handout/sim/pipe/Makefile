# Modify this line to indicate the default version to build

VERSION=std

# Comment this out if you don't have Tcl/Tk on your system

GUIMODE=-DHAS_GUI

# Modify the following line so that gcc can find the libtcl.so and
# libtk.so libraries on your system. You may need to use the -L option
# to tell gcc which directory to look in. Comment this out if you
# don't have Tcl/Tk.

#TKLIBS=-L/usr/local/lib -ltk8.4 -ltcl8.4
#TKLIBS=-L/usr/lib -ltk8.4 -ltcl8.4
TKLIBS=-L/usr/lib64 -ltk -ltcl

# Modify the following line so that gcc can find the tcl.h and tk.h
# header files on your system. Comment this out if you don't have
# Tcl/Tk.

#TKINC=-I/usr/local/include/tcl8.4
#TKINC=-I/usr/include/tcl8.4
TKINC=-I/usr/include/tcl

# Modify these two lines to choose your compiler and compile time
# flags.

CC=gcc
CFLAGS=-Wall -O2

##################################################
# You shouldn't need to modify anything below here
##################################################

MISCDIR=../misc
HCL2C=$(MISCDIR)/hcl2c
INC=$(TKINC) -I$(MISCDIR) $(GUIMODE)
LIBS=$(TKLIBS) -lm
YAS = ../misc/yas

all: psim drivers

# This rule builds the PIPE simulator
psim: psim.c sim.h pipe-$(VERSION).hcl $(MISCDIR)/isa.c $(MISCDIR)/isa.h psim.a
	# Building the pipe-$(VERSION).hcl version of PIPE
	$(HCL2C) -n pipe-$(VERSION).hcl <pipe-$(VERSION).hcl >pipe-$(VERSION).c
	$(CC) $(CFLAGS) $(INC) -o psim psim.c pipe-$(VERSION).c \
		$(MISCDIR)/isa.c psim.a $(LIBS)

# Tricky: psim.a is a library of default signal handling functions,
# each of which is only linked into psim when there is no
# corresponding signal definition in the HCL file. If there is a
# definition in the HCL file, then that definition is used
# instead. This trick exploits the way that Unix linkers use static
# libraries to resolve symbols.
psim.a: new_M_valA.c new_D_icode.c default_wb.c
	$(CC) $(CFLAGS) $(INC) -c new_M_valA.c
	$(CC) $(CFLAGS) $(INC) -c new_D_icode.c
	$(CC) $(CFLAGS) $(INC) -c default_wb.c
	ar -rs psim.a new_M_valA.o new_D_icode.o default_wb.o
	rm -f new_M_valA.o new_D_icode.o default_wb.o

# This rule builds driver programs for Part C of the Architecture Lab
drivers: 
	./gen-driver.pl -n 4 -f ncopy.ys > sdriver.ys
	../misc/yas sdriver.ys
	./gen-driver.pl -n 63 -f ncopy.ys > ldriver.ys
	../misc/yas ldriver.ys

# These are implicit rules for assembling .yo files from .ys files.
.SUFFIXES: .ys .yo
.ys.yo:
	$(YAS) $*.ys


clean:
	rm -f psim pipe-*.c *.a *.o *.exe *~ 


