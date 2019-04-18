TARGET=net
TARGETTYPE=exe
SOURCES+=$(wildcard net.c)
OFILES+=$(SOURCES:.c=.o)
EXTRACFLAGS+=$(shell pkg-config --cflags)
EXTRACFLAGS+=$(net_cflags)

EXTRALIBS=$(net_ldflags)

include $(TOPDIR)/net/directories.include
include $(TOPDIR)/makefiles/make_shared.mk
