# Makefile

PROG = projection orthogonal

all : $(PROG)

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm
LDLIBS    = -lm

#pxmtopxm : pxmtopxm.o Util.o

orthogonal :  orthogonal.o imageFormationUtils.o imageFormationUtils.o -lm
projection :  projection.o imageFormationUtils.o imageFormationUtils.o -lm

clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)

