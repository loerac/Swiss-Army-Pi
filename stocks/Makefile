CC=gcc
CFLAGS=-Wall -Werror
SOURCE=$(wildcard *.c)
OBJECT=$(patsubst %.c, %.o, %(SOURCE))
LIBS=

stocks: stocks.o
	$(CC) -o stocks stocks.o $(LIBS)

clean:
	rm -rf stocks *.o
