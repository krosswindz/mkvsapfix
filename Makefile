#
# Copyright (c) 2010 Kross Windz <krosswindz@gmail.com>
# All rights reserved.
#

# set compiler to be used
CC = i586-mingw32msvc-gcc
STRIP = i586-mingw32msvc-strip
WINDRES = i586-mingw32msvc-windres

# set compile time flags
C99 = -std=c99
CFLAGS = -Wall -Wextra -O3
GNU = -D_GNU_SOURCE
LIBRARY = -lpdcurses

# set object files needed
OBJS = mkvsapfix.o

# set the target
TARGET = mkvsapfix.exe

# compilation step
%.o : %.c
	$(CC) $(C99) $(CFLAGS) $(GNU) -c $<

all: $(TARGET)

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)

mkvsapfix.exe: $(OBJS)
	$(CC) -o $@ $? $(LIBRARY)
	$(STRIP) -s $@
