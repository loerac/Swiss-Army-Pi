
include outputs.make

#ifndef TOPDIR
        export TOPDIR=$(realpath $(shell pwd)/../)
        include ${TOPDIR}/dir.make
        include $(addsuffix /flags.make, $(addprefix ${TOPDIR}/,${all_dirs}))
#endif

export SRC_BASE=$(CURDIR)/src
export BIN_PATH=$(SRC_BASE)/bin
export LIB_PATH=$(SRC_BASE)/lib

all: ${wifi_all}
bin: ${wifi_bin}
lib: ${wifi_lib}
include: ${wifi_include}
data: ${wifi_data}

clean:
	${MAKE} -C src clean
	rm -rf ${wifi_bin} ${wifi_lib} ${wifi_lib} ${wifi_include}

.PHONY: all clean bin include data ${wifi_all}

${wifi_bin}:
	${MAKE} -C src ${@F}
	@cp -ulf $(BIN_PATH)/${@F} $@

${wifi_lib}:
	${MAKE} -C src ${@F}
	@cp -ulf $(LIB_PATH)/${@F} $@

${wifi_include}:
	cp -ln -t ${@D} $(addprefix src/include/, ${@F})
