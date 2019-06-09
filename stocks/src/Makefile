TARGET = stocks
TYPE = exe
INCLUDE =
EXTRA_CFLAGS = $(slist_cflags) $(net_cflags)
EXTRA_LDFLAGS = $(slist_ldflags) $(net_ldflags) -ljson-c -lcurl
SOURCE = $(wildcard *.c)

include $(TOPDIR)/common.mk

