CC=gcc
TARGET=../bin/wifi
CFLAGS=-Wall -Werror
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
LIBS=
CFL=$(CFLAGS) $(LIBS)

all: $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFL)

clean:
	rm -rf $(OBJECTS)
