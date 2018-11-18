CC=gcc
CFLAGS=-Wall -Werror
SOURCE=$(wildcard *.c)
OBJECT=$(patsubst %.c, %.o, %(SOURCE))
LIBS=

net: net.o
	$(CC) -o net net.o $(LIBS)

clean:
	rm -rf net *.o
