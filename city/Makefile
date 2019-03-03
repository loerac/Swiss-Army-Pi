
include outputs.make

#ifndef TOPDIR
        export TOPDIR=$(realpath $(shell pwd)/../)
        include ${TOPDIR}/dir.make
        include $(addsuffix /flags.make, $(addprefix ${TOPDIR}/,${all_dirs}))
#endif

export SRC_BASE=$(CURDIR)/src
export BIN_PATH=$(SRC_BASE)/bin
export LIB_PATH=$(SRC_BASE)/lib

all: ${city_all}
bin: ${city_bin}
lib: ${city_lib}
include: ${city_include}
data: ${city_data}

clean:
	${MAKE} -C src clean
	rm -rf ${city_bin} ${city_lib} ${city_include}

.PHONY: all clean bin lib include data ${city_all}

${city_bin}:
	${MAKE} -C src ${@F}
	@cp -ulf $(BIN_PATH)/${@F} $@

${city_lib}:
	${MAKE} -C src ${@F}
	@cp -ulf $(LIB_PATH)/${@F} $@

${city_include}:
	cp -ln -t ${@D} $(addprefix src/include/, ${@F})
