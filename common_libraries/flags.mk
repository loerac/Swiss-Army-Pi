
COMMDIR = $(TOPDIR)/common_libraries

export type_compat_cflags = -I$(COMMDIR)/compat/include/
export type_compat_ldflags = -L$(COMMDIR)/compat/lib/ -lcompat

export slist_cflags = -I$(COMMDIR)/linked_list/include/
export slist_ldflags = -L$(COMMDIR)/linked_list/lib/ -lslist

export net_cflags = -I$(COMMDIR)/net/include/
export net_ldflags = -L$(COMMDIR)/net/lib/ -lnet

export ftp_cflags = -I$(COMMDIR)/ftp/include/
export ftp_ldflags = -L$(COMMDIR)/ftp/lib/ -lftp

