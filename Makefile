
export TOPDIR=$(realpath $(shell pwd) )

export all_dirs = common_libraries city stocks

.PHONY: all
all: $(LIB_TARGET)
	$(MAKE) -C common_libraries $(@F)
	$(MAKE) -C city $(@F)
	$(MAKE) -C stocks $(@F)

.PHONY: install
install: $(LIB_TARGET)
	$(MAKE) -C common_libraries $(@F)

.PHONY: uninstall
uninstall: $(LIB_TARGET)
	$(MAKE) -C common_libraries $(@F)

.PHONY: clean
clean:
	@for dir in $(all_dirs); do \
		$(MAKE) -C $$dir clean; \
	done

