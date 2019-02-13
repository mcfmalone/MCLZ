# $Id: Makefile,v 11.12 2019/02/13 18:11:25 micha Exp micha $
# $Log: Makefile,v $
# Revision 11.12  2019/02/13 18:11:25  micha
# Removed pedantic compiler flag because it was giving a warning on __FUNCTION__, which is used in a couple of modules.
#
# Revision 11.11  2014/05/31 22:07:33  lyonsd
# Fortran compiler and flag modifications for Darwin
#
# Revision 11.10  2014/05/29 03:44:27  lyonsd
# Fixed optimization issue with labels.c/labels.o
# Added mclzldist
#
# Revision 11.9  2014/05/27 13:26:54  lyonsd
# Minor updates
#
# Revision 11.8  2014/05/23 09:45:09  lyonsd
# *** empty log message ***
#
# Revision 11.7  2014/04/30 17:16:48  lyonsd
# Added levels.c
#
# Revision 11.6  2014/04/29 15:09:33  lyonsd
# Major overhaul to Makefile to work with rewritten C code:
# lzdata.c
# lzmcro.c
# mknlrescs.c
# mknrescs.c
#
# And all other C modules and headers.
#
# Revision 11.5  2014/03/07 19:27:30  lyonsd
# Fe^26+ + H
#
# Revision 11.0  2014/03/07 18:34:16  lyonsd
# Fe^26+ + He
#
# Revision 1.4.1.1  2014/03/01 01:47:22  lyonsdav
# Fe^26+ + He
#
# Revision 1.4  2014/02/23 15:39:26  lyonsdav
# Fe^26+ + He
#
# Revision 1.3  2014/02/23 06:43:28  lyonsdav
# Added cedit rule
#
# Revision 1.2  2014/02/21 21:37:48  lyonsdav
# Mg^12+ + He
#
# Revision 1.1  2014/02/21 01:21:36  lyonsdav
# Initial revision
#
#
# FORTRAN COMPILER SETTINGS
# If you are on hal.physast.uga.ed, then uncomment this line
#FC=ifort
# If you are on Linux or Cygwin, uncomment this line
FC=gfortran
# You also may need to change these flag options, depending upon
# which system you are working.
# These flags can be used with gfortran
# FFLAGS=-O1 -std=legacy -freal-8-real-4 -Wall
FFLAGS=-O2 -std=legacy -Wall 
# These flags can be used with ifort (Intel Fotran Compiler)
#FFLAGS=-O2 -xHost -W1

# C COMPILER SETTINGS
CC=gcc
WFLAGS=-W \
	   -Wall \
	   -Werror \
	   -Wextra \
	   -Wshadow \
	   -Wcast-qual \
	   -Wcast-align \
	   -Wwrite-strings \
	   -Wpointer-arith \
	   -Wnested-externs \
	   -Wstrict-prototypes \
	   -Wmissing-prototypes
OPTIMIZATION=2
CFLAGS=$(WFLAGS) \
	   -g \
	   -ansi \
#	   -pedantic
	   -Dinline= \
	   -fno-common \
	   -fshort-enums \
	   -O$(OPTIMIZATION)
CFLAGS4=$(WFLAGS) \
	   -g \
	   -ansi \
#	   -pedantic
	   -Dinline= \
	   -fno-common \
	   -fshort-enums
LFLAGS=-lm
TARGETS=mknlrescs \
		mclzldist \
		mknrescs \
		lzmcro \
		lzdata \
		levels \
		degen
OBJ1=couplings.o \
	 crossing.o \
	 element.o \
	 lzdata.o \
	 utils.o \
	 lzcxs.o \
	 nist.o
OBJ2=lzmcro.o
OBJ3=mknlrescs.o \
	 csheader.o \
	 xsection.o \
	 labels.o \
	 csdata.o \
	 utils.o
OBJ4=mknrescs.o \
	 csheader.o \
	 xsection.o \
	 labels.o \
	 csdata.o \
	 utils.o
OBJ5=levels.o \
	 utils.o
OBJ6=degen.o \
	 utils.o \
	 nist.o
OBJ7=mclzldist.o
OBJECTS=$(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7)
SOURCES=mknlrescs.c \
		mclzldist.f \
		couplings.c \
		mknrescs.c \
		xsection.c \
		crossing.c \
		csheader.c \
		element.c \
		csdata.c \
		labels.c \
		lzmcro.c \
		lzdata.c \
		levels.c \
		degen.c \
		utils.c \
		lzcxs.c \
		nist.c
HEADERS=couplings.h \
		xsection.h \
		csheader.h \
		crossing.h \
		element.h \
		csdata.h \
		labels.h \
		utils.h \
		lzcxs.h \
		nist.h
MANPAGES=lzdata.1\
		 lzmcro.1
SCRIPTS=mclz.sh \
		fnist1.pl \
		fnist2.pl
DATAFILES=elements.dat
ALL=$(SOURCES) $(HEADERS) $(MANPAGES) $(SCRIPTS) $(DATAFILES)

all: $(TARGETS)

mclzldist : $(OBJ7)
	$(FC) $(FFLAGS) -o $@ $^

mclzldist.o: mclzldist.f
	$(FC) $(FFLAGS) -c $^

levels: $(OBJ5)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

degen: $(OBJ6)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

mknlrescs: $(OBJ3)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^ 
#	@rm $(OBJ1)

mknrescs: $(OBJ4)
	$(CC) $(CFLAGS4) $(LFLAGS) -o $@ $^
#	@rm $(OBJ2)

lzmcro:	$(OBJ2)
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

lzdata: $(OBJ1)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
#	@rm $(OBJ3)

labels.o: OPTIMIZATION=0

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

install: $(TARGETS) ${SCRIPTS}
	mv $^ ~/bin

edit: Makefile $(SOURCES) $(HEADERS)
	gvim -p17 $(SOURCES)
	gvim -p10 $(HEADERS)
	gvim -p3  $(MANPAGES) $(DATAFILES)
	gvim -p3  $(SCRIPTS)

ci:
	ci $(ALL)

co:
	co $(ALL)
	chmod +x $(SCRIPTS)

lock:
	rcs -l $(ALL)

clean:
	@rm -f $(OBJECTS) $(TARGETS)
