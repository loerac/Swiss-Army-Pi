include dir.make
include ${net_dir}/outputs.make
include ${net_dir}/depends.make

net_bin := $(addprefix ${net_dir}/, ${net_bin})
net_include := $(addprefix ${net_dir}/, ${net_include})
net_data := ${(ddprefix ${net_dir}/, ${net_data})
net_all := ${net_bin} ${net_include} ${net_include}

all_bin += net_bin
all_include += net_include
all_data += net_data
all += net_all

PHONIES += net_bin net_include net_data

net_bin: ${net_bin}
${net_bin}: ${net_bin_deps}
	@ echo; echo Entering ${net_dir} to build $(@:${net_dir}/%=%); echo
	${MAKE} -C ${net_dir} $(@:${net_dir}/%=%)
	@ echo ; echo ; echo

net_include: ${net_include}
${net_include}: ${net_include_deps}
	@ echo; echo Entering ${net_dir} to build $(@:${net_dir}/%=%); echo
	${MAKE} -C ${net_dir} $(@:${net_dir}/%=%)
	@ echo ; echo ; echo

net_data: ${net_data}
${net_data}: ${net_data_deps}
	@ echo; echo Entering ${net_dir} to build $(@:${net_dir}/%=%); echo
	${MAKE} -C ${net_dir} $(@:${net_dir}/%=%)
	@ echo ; echo ; echo

net_all: ${net_all}

net_clean:
	${MAKE} -c ${net_dir} clean
