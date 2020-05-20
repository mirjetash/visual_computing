#######################################
# Makefile PBM                        #
#                                     #
# E.B.                                #
#######################################


PROG = computing_gradient user_threshold non_maximum_supression hysteresis

all : $(PROG)

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm
LDLIBS    = -lm

#pxmtopxm : pxmtopxm.o Util.o

computing_gradient : computing_gradient.o Util.o
user_threshold : user_threshold.o Util.o
canny_approach : canny_approach.o Util.o
non_maximum_supression: non_maximum_supression.o Util.o
hysteresis: hysteresis.o Util.o

clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)

