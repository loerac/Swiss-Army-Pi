
include outputs.make

#ifndef TOPDIR
        export TOPDIR=$(realpath $(shell pwd)/../)
        include ${TOPDIR}/dir.make
        include $(addsuffix /flags.make, $(addprefix ${TOPDIR}/,${all_dirs}))
#endif

export SRC_BASE=$(CURDIR)/src
export BIN_PATH=$(SRC_BASE)/bin
export LIB_PATH=$(SRC_BASE)/lib

all: ${net_all}
bin: ${net_bin}
lib: ${net_lib}
include: ${net_include}
data: ${net_data}

clean:
	${MAKE} -C src clean
	rm -rf ${net_bin} ${net_lib} ${net_lib} ${net_include}

.PHONY: all clean bin include data ${net_all}

${net_bin}:
	${MAKE} -C src ${@F}
	@cp -ulf $(BIN_PATH)/${@F} $@

${net_lib}:
	${MAKE} -C src ${@F}
	@cp -ulf $(LIB_PATH)/${@F} $@

${net_include}:
	cp -ln -t ${@D} $(addprefix src/include/, ${@F})
