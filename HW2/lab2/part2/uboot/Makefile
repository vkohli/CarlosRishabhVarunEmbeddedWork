PREFIX  = arm-linux-
CC      = $(PREFIX)gcc
CFLAGS  = -O2 -ffixed-r8 -ffreestanding -Wall -Werror -nostdinc -I include -g

default: stubs.o

.PHONY: default clobber

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clobber:
	rm -f stubs.o
