
common_libs = compat linked_list net

.PHONY: all
all: $(LIB_TARGET)
	$(MAKE) -C compat $(@F)
	$(MAKE) -C linked_list $(@F)
	$(MAKE) -C net $(@F)

.PHONY: install
install: $(LIB_TARGET)
	$(MAKE) -C compat $(@F)
	$(MAKE) -C linked_list $(@F)
	$(MAKE) -C net $(@F)

.PHONY: uninstall
uninstall: $(LIB_TARGET)
	$(MAKE) -C compat $(@F)
	$(MAKE) -C linked_list $(@F)
	$(MAKE) -C net $(@F)

.PHONY: clean
clean:
	@for dir in $(common_libs); do \
		$(MAKE) -C $$dir clean; \
	done
