TARGET=city
SOURCES=$(wildcard *.c)
OFILES=$(SOURCES:.c=.o)
EXTRACFLAGS=$(shell pkg-config --cflags)
EXTRALIBS=$(shell pkg-config --libs ) -ljson-c -lcurl

include $(TOPDIR)/city/directories.include
include $(TOPDIR)/makefiles/make_shared.mk
