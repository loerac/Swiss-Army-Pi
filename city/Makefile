CC=gcc
CFLAGS=-Wall -Werror
SOURCE=$(wildcard *.c)
OBJECT=$(patsubst %.c, %.o, %(SOURCE))
LIBS=-ljson-c -lcurl

city: city.o city_curl.o city_file.o city_parse.o
	$(CC) -o city city.o city_curl.o city_file.o city_parse.o $(LIBS)

clean:
	rm -rf city *.o
