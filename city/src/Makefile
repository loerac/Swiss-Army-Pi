TARGET = city
TYPE = exe
INCLUDE =
EXTRA_CFLAGS = $(slist_cflags) $(net_cflags) $(type_compat_cflags) $(ftp_cflags)
EXTRA_LDFLAGS = -ljson-c -lcurl $(slist_ldflags) $(net_ldflags) $(type_compat_ldflags) $(ftp_ldflags)
SOURCE = $(wildcard *.c)

include $(TOPDIR)/common.mk
