INCDIR=/home/atkrye/Documents/PGP-2_Install//include
LIBDIR=/home/atkrye/Documents/PGP-2_Install//lib
OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
CC=gcc

CFLAGS = -I$(INCDIR) -L$(LIBDIR) -fomit-frame-pointer -O2 -Wall -Wextra -lgp2 -lm

default:	$(OBJECTS)
		$(CC) $(OBJECTS) $(CFLAGS) -o EDGrun

%.o:		%.c
		$(CC) -c $(CFLAGS) -o $@ $<

clean:	
		rm *
