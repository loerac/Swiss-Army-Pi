TARGET = nis
TYPE = exe
INCLUDE =
EXTRA_CFLAGS = $(net_cflags) $(type_compat_cflags) $(ftp_cflags)
EXTRA_LDFLAGS = -ljson-c -lcurl $(net_ldflags) $(type_compat_ldflags) $(ftp_ldflags)
SOURCE = $(wildcard *.c)

include $(TOPDIR)/common.mk

