CC=gcc
TARGET=../bin/city
CFLAGS=-Wall -Werror
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
LIBS=-ljson-c -lcurl
CFL=$(CFLAGS) $(LIBS)

all: $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFL)

clean:
	rm -rf $(OBJECTS)
