
common_libs = compat linked_list net ftp

.PHONY: all
all: $(LIB_TARGET)
	$(MAKE) -C compat $(@F)
	$(MAKE) -C linked_list $(@F)
	$(MAKE) -C net $(@F)
	$(MAKE) -C ftp $(@F)

.PHONY: install
install: $(LIB_TARGET)
	$(MAKE) -C compat $(@F)
	$(MAKE) -C linked_list $(@F)
	$(MAKE) -C net $(@F)
	$(MAKE) -C ftp $(@F)

.PHONY: uninstall
uninstall: $(LIB_TARGET)
	$(MAKE) -C compat $(@F)
	$(MAKE) -C linked_list $(@F)
	$(MAKE) -C net $(@F)
	$(MAKE) -C ftp $(@F)

.PHONY: clean
clean:
	@for dir in $(common_libs); do \
		$(MAKE) -C $$dir clean; \
	done
