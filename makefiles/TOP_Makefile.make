.DEFAULT_GOAL:=all

export TOPDIR=$(realpath $(shell pwd) )
export EXPORT_LIBRARY ?= /usr/local/lib

include *_Makefile.make
include $(addsuffix /flags.make,${all_dirs})

.PHONY: all install uninstall clean ${PHONIES}
all: ${all}

install:
	@for dir in ${all_libs}; do \
		${MAKE} -C $$dir install; \
	done

uninstall:
	@for dir in ${all_libs}; do \
		${MAKE} -C $$dir uninstall; \
	done

clean:
	@for dir in ${all_dirs}; do \
		${MAKE} -C $$dir clean; \
	done
