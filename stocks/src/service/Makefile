TARGET=stocks
TARGETTYPE=exe
SOURCES=$(wildcard *.c)
OFILES=$(SOURCES:.c=.o)
EXTRACFLAGS=$(shell pkg-config --cflags)
EXTRACFLAGS+=${net_cflags}

EXTRALIBS=$(shell pkg-config --libs ) -ljson-c -lcurl
EXTRALIBS+=${net_ldflags}

include $(TOPDIR)/stocks/directories.include
include $(TOPDIR)/makefiles/make_shared.mk
