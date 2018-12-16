CC=gcc
TARGET=city
CFLAGS=-Wall -Werror
TARGETS=cfile cparse ccurl
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
LIBS=-ljson-c -lcurl
CFL=$(CFLAGS) $(LIBS)

all: $(OBJECTS) 
	$(CC) -o $(TARGET) $(OBJECTS) $(CFL)

file: city_file.o city.o
	$(CC) -o cfile city_file.o city.o $(CFL)

parse: city_parse.o city.o city_file.o city_curl.o
	$(CC) -o cparse city_parse.o city_file.o city_curl.o city.o $(CFL)

curl: city_curl.o city.o
	$(CC) -o ccurl city_curl.o city.o $(CFL)

clean:
	rm -rf $(TARGET) $(TARGETS) $(OBJECTS)
