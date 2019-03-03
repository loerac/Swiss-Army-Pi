TARGET=net
SOURCES=$(wildcard *.c)
OFILES=$(SOURCES:.c=.o)
EXTRACFLAGS=$(shell pkg-config --cflags)

include $(TOPDIR)/net/directories.include
include $(TOPDIR)/makefiles/make_shared.mk
