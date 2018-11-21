CC=gcc
TARGET=city
CFLAGS=-Wall -Werror
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
LIBS=-ljson-c -lcurl

all: $(OBJECTS) 
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	rm -rf $(TARGET) city.o city_curl.o city_file.o city_parse.o
