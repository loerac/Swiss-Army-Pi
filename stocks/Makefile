# Add the top directory to the beginning all the parts and
# add '/flags.mk' to the end of those parts
include $(addsuffix /flags.mk, $(addprefix $(TOPDIR)/,$(all_dirs)))

export OUTPUT_BIN = $(CURDIR)/bin

BUILD = src
all: $(BUILD)

.PHONY: all $(BUILD)
src:
	$(MAKE) -C src $(@F)

install:
	$(MAKE) -C src $(@F)

uninstall:
	$(MAKE) -C src $(@F)

.PHONY: clean
clean:
	$(MAKE) -C src $(@F)
