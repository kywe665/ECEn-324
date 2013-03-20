####################################################
# Students' Makefile for the CS:APP Performance Lab
####################################################

# You may edit TEAM to be your submission name. It should be of the
# form "netid" or "netid1+netid2".
TEAM = bovik
VERSION = 1

##################################################
# You shouldn't need to modify anything below here
##################################################

# Compiler information.
#CC = gcc4
CC = gcc
CFLAGS = -Wall -O2
LIBS = -lm

OBJS = driver.o kernels.o fcyc.o clock.o

# Submission information.
SENDMAIL = /usr/sbin/sendmail -bm
#ADDRESS = cs324ta@cs.byu.edu
#ADDRESS = ecen324@ee.byu.edu winter 2009 changed to et.byu.edu
ADDRESS = ecen324@et.byu.edu

#HOME = /users/ta/cs324ta/handin/perflab
HOME = /auto/grp2/ece324web/handin/perflab

# Rules.
all: driver

driver: $(OBJS) fcyc.h clock.h defs.h config.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o driver

kernels.o: kernels.c config.h defs.h
	$(CC) $(CFLAGS) -ansi -c kernels.c

handin: header.txt
	@test xbovik != x"$(TEAM)" \
		|| (echo Remember to set TEAM! && false)
	@test ! -e $(TEAM)-$(VERSION)-kernels.c \
		|| (echo Remember to increase VERSION! && false)
	cp kernels.c $(TEAM)-$(VERSION)-kernels.c
	(cat header.txt; \
	 tar cjf - $(TEAM)-$(VERSION)-kernels.c | \
	 uuencode $(HOME)/handin/$(TEAM)-$(VERSION).tar.bz2) | \
	 $(SENDMAIL) $(ADDRESS)

header.txt:
	@echo Subject: perflab handin > header.txt
	@echo >> header.txt

clean:
	-rm -f $(OBJS) driver core *~ *.o

veryclean: clean
	rm -f header.txt *-*-kernels.c
