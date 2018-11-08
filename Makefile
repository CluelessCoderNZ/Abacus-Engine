# This is a temporary make file that we are not the author of
# Based From: https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html
# 
# TODO(Connor): Replace make file with our own version 

#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        builds for default linux
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define default make file location
DEFAULT_MAKE = linux


# define the executable location
WIN_DIR = bin
LINUX_DIR = bin
OSX_DIR = bin

# define the executable file 
MAIN = abacus

# define libraries
WIN_LIBS =
LINUX_LIBS = -lX11
OSX_LIBS = 


# define any compile-time flags
CFLAGS = 

# OS Specific compile-time flags
WIN_CFLAGS = -DOS_WIN32
LINUX_CFLAGS = -DOS_LINUX
OSX_CFLAGS = -DOS_OSX

# define the general source files
SRCS = main.cpp

# OS Specific Source files
WIN_SRCS = 
LINUX_SRCS = linux_platform.cpp
OSX_SRCS =


WIN_OBJS = $(SRCS:.c=.o) $(WIN_SRCS:.c=.o)
LINUX_OBJS = $(SRCS:.c=.o) $(LINUX_SRCS:.c=.o)
OSX_OBJS = $(SRCS:.c=.o) $(OSX_SRCS:.c=.o)


#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(DEFAULT_MAKE)


win32:	$(WIN_OBJS)
		$(CC) $(CFLAGS) $(WIN_CFLAGS) -o $(WIN_DIR)\$(MAIN).exe $(WIN_OBJS) $(WIN_LIBS)

linux:	$(LINUX_OBJS)
		$(CC) $(CFLAGS) $(LINUX_CFLAGS) -o $(LINUX_DIR)/$(MAIN) $(LINUX_OBJS) $(LINUX_LIBS)

osx:	$(OSX_OBJS)
		$(CC) $(CFLAGS) $(OSX_CFLAGS) -o $(OSX_DIR)/$(MAIN) $(OSX_OBJS) $(OSX_LIBS)


# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
