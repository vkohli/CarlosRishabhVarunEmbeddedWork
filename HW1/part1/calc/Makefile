CC = gcc
CFLAGS = -Wall -Werror

objects = calc.o math.o

default: calc

.PHONY: default clean clobber

calc: $(objects)
	$(CC) -o $@ $^

calc.o: calc.c math.h
math.o: math.c math.h

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f calc
