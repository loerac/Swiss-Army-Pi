CC=gcc
TARGET=city
CFLAGS=-Wall -Werror
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
LIBS=-ljson-c -lcurl

all: $(OBJECTS) 
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	rm -rf $(TARGET) $(OBJECTS)
