include dir.make
include ${stocks_dir}/outputs.make
include ${stocks_dir}/depends.make

stocks_bin := $(addprefix ${stocks_dir}/, ${stocks_bin})
stocks_lib := $(addprefix ${stocks_dir}/, ${stocks_lib})
stocks_include := $(addprefix ${stocks_dir}/, ${stocks_include})
stocks_data := ${(ddprefix ${stocks_dir}/, ${stocks_data})
stocks_all := ${stocks_bin} ${stocks_include} ${stocks_include}

all_bin += stocks_bin
all_lib += stocks_lib
all_include += stocks_include
all_data += stocks_data
all += stocks_all

PHONIES += stocks_bin stocks_lib stocks_include stocks_data

stocks_bin: ${stocks_bin}
${stocks_bin}: ${stocks_bin_deps}
	@ echo; echo Entering ${stocks_dir} to build $(@:${stocks_dir}/%=%); echo
	mkdir -p ${stocks_dir}/bin
	${MAKE} -C ${stocks_dir} $(@:${stocks_dir}/%=%)
	@ echo ; echo ; echo

stocks_lib: ${stocks_lib}
${stocks_lib}: ${stocks_lib_deps}
	@ echo; echo Entering ${stocks_dir} to build $(@:${stocks_dir}/%=%); echo
	mkdir -p ${stocks_dir}/lib
	${MAKE} -C ${stocks_dir} $(@:${stocks_dir}/%=%)
	@ echo ; echo ; echo

stocks_include: ${stocks_include}
${stocks_include}: ${stocks_include_deps}
	@ echo; echo Entering ${stocks_dir} to build $(@:${stocks_dir}/%=%); echo
	mkdir -p ${stocks_dir}/include
	${MAKE} -C ${stocks_dir} $(@:${stocks_dir}/%=%)
	@ echo ; echo ; echo

stocks_data: ${stocks_data}
${stocks_data}: ${stocks_data_deps}
	@ echo; echo Entering ${stocks_dir} to build $(@:${stocks_dir}/%=%); echo
	${MAKE} -C ${stocks_dir} $(@:${stocks_dir}/%=%)
	@ echo ; echo ; echo

stocks_all: ${stocks_all}

stocks_clean:
	${MAKE} -c ${stocks_dir} clean
