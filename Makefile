VERSION = 0.1

TCLLIB=/usr/lib/tcl8.6
TCLINC=/usr/include/tcl8.6
INCLUDES = -I$TCLINC
LIBRARIES = -lm -ltiff -ltclstub8.6 -L$TCLLIB
BINDIR = /usr/local/bin
CC = gcc
#CC= clang

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
