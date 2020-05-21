#######################################
# Makefile PBM                        #
#                                     #
# E.B.                                #
#######################################


PROG = color_intensity_kmeans spatial_color_kmeans

all : $(PROG)

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm
LDLIBS    = -lm

#pxmtopxm : pxmtopxm.o Util.o

color_intensity_kmeans : color_intensity_kmeans.o Util.o
spatial_color_kmeans : spatial_color_kmeans.o Util.o

clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)

