VERSION = 0.1

# Linux (Ubuntu 12.04)
TCLLIB=/usr/lib/tcl8.5
TCLINC=/usr/include/tcl8.5
INCLUDES = -I$TCLINC
LIBRARIES = -lm -ltiff -ltclstub8.5 -L$TCLLIB
BINDIR = /usr/local/bin
CC = gcc-4.5
#CC= clang

# Linux (Ubuntu 12.04, MPI)
#INCLUDES = -I/usr/include/tcl8.5 -I/usr/lib/openmpi/include/
#LIBRARIES = -lm -ltiff
#BINDIR = /usr/local/bin
#EXTRA_FLAGS = -DMPI
#OMPI_CC=gcc-4.5
#CC = mpicc.openmpi

#SRC = tiffread-3.c
OBJ = $(SRC:.c=.o)
STRIP = strip
CP = cp
FLAGS = -fPIC -shared -o libSE_TCL_STUBS
RM = rm
TAR = tar
MKDIR = mkdir

all: libtiffread-1.c
	$(CC) $(FLAGS) $(EXTRA_FLAGS) $(INCLUDES) -o libtiffread1.so libtiffread-1.c $(LIBRARIES)
clean:
	$(RM) -f libtiffread1.so
#install:
#	$(STRIP) simpson
#	$(CP) simpson $(BINDIR)

