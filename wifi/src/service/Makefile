TARGET=wifi
SOURCES=$(wildcard wifi.c)
OFILES=$(SOURCES:.c=.o)
EXTRACFLAGS=$(shell pkg-config --cflags) -ljson-c -lcurl

include $(TOPDIR)/wifi/directories.include
include $(TOPDIR)/makefiles/make_shared.mk
