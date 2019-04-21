TARGET=libnet
TARGETTYPE=so
SOURCES=$(wildcard *.c)
OFILES=$(SOURCES:.c=.o)

EXTRACFLAGS=${net_cflags}

EXTRALIBS+=${net_ldflags}

include $(TOPDIR)/net/directories.include
include $(TOPDIR)/makefiles/make_shared.mk
