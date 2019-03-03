include dir.make
include ${wifi_dir}/outputs.make
include ${wifi_dir}/depends.make

wifi_bin := $(addprefix ${wifi_dir}/, ${wifi_bin})
wifi_include := $(addprefix ${wifi_dir}/, ${wifi_include})
wifi_data := ${(ddprefix ${wifi_dir}/, ${wifi_data})
wifi_all := ${wifi_bin} ${wifi_include} ${wifi_include}

all_bin += wifi_bin
all_include += wifi_include
all_data += wifi_data
all += wifi_all

PHONIES += wifi_bin wifi_include wifi_data

wifi_bin: ${wifi_bin}
${wifi_bin}: ${wifi_bin_deps}
	@ echo; echo Entering ${wifi_dir} to build $(@:${wifi_dir}/%=%); echo
	${MAKE} -C ${wifi_dir} $(@:${wifi_dir}/%=%)
	@ echo ; echo ; echo

wifi_include: ${wifi_include}
${wifi_include}: ${wifi_include_deps}
	@ echo; echo Entering ${wifi_dir} to build $(@:${wifi_dir}/%=%); echo
	${MAKE} -C ${wifi_dir} $(@:${wifi_dir}/%=%)
	@ echo ; echo ; echo

wifi_data: ${wifi_data}
${wifi_data}: ${wifi_data_deps}
	@ echo; echo Entering ${wifi_dir} to build $(@:${wifi_dir}/%=%); echo
	${MAKE} -C ${wifi_dir} $(@:${wifi_dir}/%=%)
	@ echo ; echo ; echo

wifi_all: ${wifi_all}

wifi_clean:
	${MAKE} -c ${wifi_dir} clean
