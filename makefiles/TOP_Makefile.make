.DEFAULT_GOAL:=all

export TOPDIR=$(realpath $(shell pwd) )
export EXPORT_LIBRARY ?= /usr/local/lib
export EXPORT_BINARY  ?= /usr/local/bin

include *_Makefile.make
include $(addsuffix /flags.make,${all_dirs})

.PHONY: all install uninstall clean ${PHONIES}
all: ${all}

install:
	@for dir in ${all_libs}; do \
		${MAKE} -C $$dir install; \
	done
	@echo "export LD_LIBRARY_PATH=/usr/local/lib/:$(LD_LIBRARY_PATH)"
	@for dir in ${all_bins}; do \
		${MAKE} -C $$dir install; \
	done

uninstall:
	@for dir in ${all_libs}; do \
		${MAKE} -C $$dir uninstall; \
	done
	@for dir in ${all_bins}; do \
		${MAKE} -C $$dir uninstall; \
	done

clean:
	@for dir in ${all_dirs}; do \
		${MAKE} -C $$dir clean; \
	done
