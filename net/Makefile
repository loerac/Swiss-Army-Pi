
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
api: ${net_api}
include: ${net_include}
data: ${net_data}

install:
	cp -a ${net_lib} $(EXPORT_LIBRARY)
	cp -a ${net_bin} $(EXPORT_BINARY)

uninstall:
	rm -rf $(EXPORT_LIBRARY)/$(net_lib:lib/%=%)
	rm -rf $(EXPORT_BINARY)/$(net_bin:bin/%=%)

clean:
	${MAKE} -C src clean
	rm -rf ${net_bin} ${net_lib} ${net_api} ${net_lib} ${net_include}

.PHONY: all clean bin lib include data ${net_all}

${net_bin}:
	${MAKE} -C src ${@F}
	@cp -ulf $(BIN_PATH)/${@F} $@

${net_lib}:
	${MAKE} -C src ${@F}
	@cp -ulf $(LIB_PATH)/${@F} $@

${net_include}:
	cp -ln -t ${@D} $(addprefix src/include/, ${@F})

