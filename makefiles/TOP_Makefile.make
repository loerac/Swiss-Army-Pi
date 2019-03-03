.DEFAULT_GOAL:=all

export TOPDIR=$(realpath $(shell pwd) )

include *_Makefile.make
include $(addsuffix /flags.make,${all_dirs})

.PHONY: all clean ${PHONIES}
all: ${all}

clean:
	@for dir in ${all_dirs}; do \
		${MAKE} -C $$dir clean; \
	done
