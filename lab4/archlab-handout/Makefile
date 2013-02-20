####################################################
# Students' Makefile for the CS:APP Architecture Lab
####################################################

# Default team name and version number
#TEAM = NO_TEAM
#VERSION = 1

# How to mail the lab parts in.
SENDMAIL = /usr/sbin/sendmail -bm
#ADDRESS = cs324ta@cs.byu.edu
#ADDRESS = ecen324@ee.byu.edu changed to et.byu.edu winter 2009
ADDRESS = ecen324@et.byu.edu
#HOME = /users/ta/cs324ta/handin/archlab
HOME = /auto/grp2/ece324web/handin/archlab

# The files to submit.
PARTA_FILES = sim/misc/sum.ys sim/misc/rsum.ys sim/misc/copy.ys
PARTB_FILES = sim/seq/seq-full.hcl
PARTC_FILES = sim/pipe/ncopy.ys sim/pipe/pipe-full.hcl

# Use this rule to hand in all three parts at once ("make")
all: handin-parta handin-partb handin-partc

# Use this rule to hand in Part A ("make handin-parta")
handin-parta: header.txt $(PARTA_FILES)
ifndef TEAM
	@echo "Please put a TEAM name"
	@exit 1
endif
ifndef VERSION
	@echo "Please put a VERSION"
	@exit 2
endif
	@test ! -e $(TEAM)-$(VERSION)-sum.ys \
		|| (echo Remember to increase VERSION! && false)
	@test ! -e $(TEAM)-$(VERSION)-rsum.ys \
		|| (echo Remember to increase VERSION! && false)
	@test ! -e $(TEAM)-$(VERSION)-copy.ys \
		|| (echo Remember to increase VERSION! && false)
	cp sim/misc/sum.ys $(TEAM)-$(VERSION)-sum.ys
	cp sim/misc/rsum.ys $(TEAM)-$(VERSION)-rsum.ys
	cp sim/misc/copy.ys $(TEAM)-$(VERSION)-copy.ys
	(cat header.txt; \
	 tar cf - $(TEAM)-$(VERSION)-sum.ys \
		$(TEAM)-$(VERSION)-rsum.ys \
		$(TEAM)-$(VERSION)-copy.ys | \
	 uuencode $(HOME)/parta/handin/$(TEAM)-$(VERSION).tar) | \
	 $(SENDMAIL) $(ADDRESS)

# Use this rule to handin Part B ("make handin-partb")
handin-partb: header.txt $(PARTB_FILES)
ifndef TEAM
	@echo "Please put a TEAM name"
	@exit 1
endif
ifndef VERSION
	@echo "Please put a VERSION"
	@exit 2
endif
	@test ! -e $(TEAM)-$(VERSION)-seq-full.hcl \
		|| (echo Remember to increase VERSION! && false)
	cp sim/seq/seq-full.hcl $(TEAM)-$(VERSION)-seq-full.hcl
	(cat header.txt; \
	 tar cf - $(TEAM)-$(VERSION)-seq-full.hcl | \
	 uuencode $(HOME)/partb/handin/$(TEAM)-$(VERSION).tar) | \
	 $(SENDMAIL) $(ADDRESS)

# Use this rule to handin Part C ("make handin-partc")
handin-partc: header.txt $(PARTC_FILES)
ifndef TEAM
	@echo "Please put a TEAM name"
	@exit 1
endif
ifndef VERSION
	@echo "Please put a VERSION"
	@exit 2
endif
	@test ! -e $(TEAM)-$(VERSION)-ncopy.ys \
		|| (echo Remember to increase VERSION! && false)
	@test ! -e $(TEAM)-$(VERSION)-pipe-full.hcl \
		|| (echo Remember to increase VERSION! && false)
	cp sim/pipe/ncopy.ys $(TEAM)-$(VERSION)-ncopy.ys
	cp sim/pipe/pipe-full.hcl $(TEAM)-$(VERSION)-pipe-full.hcl
	(cat header.txt; \
	 tar cf - $(TEAM)-$(VERSION)-ncopy.ys \
		$(TEAM)-$(VERSION)-pipe-full.hcl | \
	 uuencode $(HOME)/partc/handin/$(TEAM)-$(VERSION).tar) | \
	 $(SENDMAIL) $(ADDRESS)

header.txt:
	@echo Subject: Archlab handin > header.txt
	@echo >> header.txt

clean:
	rm -f *~ *.o

veryclean: clean
	rm -f handin *.ys *.hcl header.txt
