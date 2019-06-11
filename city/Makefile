# Add the top directory to the beginning all the parts and
# add '/flags.mk' to the end of those parts
include $(addsuffix /flags.mk, $(addprefix $(TOPDIR)/,$(all_dirs)))

export OUTPUT_BIN = $(CURDIR)/bin
export OUTPUT_DATA = $(CURDIR)/data

BUILD = src include
all: $(BUILD)

.PHONY: all $(BUILD)
src:
	$(MAKE) -C src $(@F)

install:
	$(MAKE) -C src $(@F)

install_data:
	$(MAKE) -C data $(@F)

uninstall:
	$(MAKE) -C src $(@F)

uninstall_data:
	$(MAKE) -C data $(@F)

.PHONY: clean
clean:
	$(MAKE) -C src $(@F)
