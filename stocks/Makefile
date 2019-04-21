
include outputs.make

#ifndef TOPDIR
        export TOPDIR=$(realpath $(shell pwd)/../)
        include ${TOPDIR}/dir.make
        include $(addsuffix /flags.make, $(addprefix ${TOPDIR}/,${all_dirs}))
#endif

export SRC_BASE=$(CURDIR)/src
export BIN_PATH=$(SRC_BASE)/bin
export LIB_PATH=$(SRC_BASE)/lib

all: ${stocks_all}
bin: ${stocks_bin}
lib: ${stocks_lib}
include: ${stocks_include}
data: ${stocks_data}

install:
	cp -a ${stocks_bin} $(EXPORT_BINARY)

uninstall:
	rm -rf $(EXPORT_BINARY)/$(stocks_bin:bin/%=%)

clean:
	${MAKE} -C src clean
	rm -rf ${stocks_bin} ${stocks_lib} ${stocks_include}

.PHONY: all clean bin lib include data ${stocks_all}

${stocks_bin}:
	${MAKE} -C src ${@F}
	@cp -ulf $(BIN_PATH)/${@F} $@

${stocks_lib}:
	${MAKE} -C src ${@F}
	@cp -ulf $(LIB_PATH)/${@F} $@

${stocks_include}:
	cp -ln -t ${@D} $(addprefix src/include/, ${@F})
