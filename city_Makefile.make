include dir.make
include ${city_dir}/outputs.make
include ${city_dir}/depends.make

city_bin := $(addprefix ${city_dir}/, ${city_bin})
city_lib := $(addprefix ${city_dir}/, ${city_lib})
city_include := $(addprefix ${city_dir}/, ${city_include})
city_data := ${(ddprefix ${city_dir}/, ${city_data})
city_all := ${city_bin} ${city_include} ${city_include}

all_bin += city_bin
all_lib += city_lib
all_include += city_include
all_data += city_data
all += city_all

PHONIES += city_bin city_lib city_include city_data

city_bin: ${city_bin}
${city_bin}: ${city_bin_deps}
	@ echo; echo Entering ${city_dir} to build $(@:${city_dir}/%=%); echo
	mkdir -p ${city_dir}/bin
	${MAKE} -C ${city_dir} $(@:${city_dir}/%=%)
	@ echo ; echo ; echo

city_lib: ${city_lib}
${city_lib}: ${city_lib_deps}
	@ echo; echo Entering ${city_dir} to build $(@:${city_dir}/%=%); echo
	mkdir -p ${city_dir}/lib
	${MAKE} -C ${city_dir} $(@:${city_dir}/%=%)
	@ echo ; echo ; echo

city_include: ${city_include}
${city_include}: ${city_include_deps}
	@ echo; echo Entering ${city_dir} to build $(@:${city_dir}/%=%); echo
	mkdir -p ${city_dir}/include
	${MAKE} -C ${city_dir} $(@:${city_dir}/%=%)
	@ echo ; echo ; echo

city_data: ${city_data}
${city_data}: ${city_data_deps}
	@ echo; echo Entering ${city_dir} to build $(@:${city_dir}/%=%); echo
	${MAKE} -C ${city_dir} $(@:${city_dir}/%=%)
	@ echo ; echo ; echo

city_all: ${city_all}

city_clean:
	${MAKE} -c ${city_dir} clean
