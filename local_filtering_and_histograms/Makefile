#######################################
# Makefile PBM                        #
#                                     #
# E.B.                                #
#######################################


#PROG = binomial_filter

#all : $(PROG)

all: binomial_filter median 

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm

#pxmtopxm : pxmtopxm.o Util.o

binomial_filter : binomial_filter.o Util.o
median : median.o Util.o

clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)

