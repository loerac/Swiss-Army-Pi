
export TOPDIR=$(realpath $(shell pwd) )

export all_dirs = common_libraries ais city stocks

# Directories
export SAP	= /SAP
export CUST	= $(SAP)/custom
export CONF	= $(SAP)/config
export AIS_CONF = $(CONF)/ais
export CITY_CUST = $(CUST)/city
export CITY_CONF = $(CONF)/city
export STOCK_CUST = $(CUST)/stocks
export STOCK_CONF = $(CONF)/stocks

all_files = $(AIS_CONF) $(CITY_CUST) $(CITY_CONF) $(STOCK_CUST) $(STOCK_CONF)

.PHONY: all
all: $(LIB_TARGET)
	$(MAKE) -C common_libraries $(@F)
	$(MAKE) -C ais $(@F)
	$(MAKE) -C city $(@F)
	$(MAKE) -C stocks $(@F)

.PHONY: install
install: $(LIB_TARGET)
	$(MAKE) -C common_libraries $(@F)

.PHONY: install_data
install_data: $(LIB_TARGET)
	$(MAKE) -C ais $(@F)
	$(MAKE) -C city $(@F)
	$(MAKE) -C stocks $(@F)

.PHONY: uninstall
uninstall: $(LIB_TARGET)
	$(MAKE) -C common_libraries $(@F)

.PHONY: uninstall_data
uninstall_data: $(LIB_TARGET)
	$(MAKE) -C ais $(@F)
	$(MAKE) -C city $(@F)
	$(MAKE) -C stocks $(@F)

.PHONY: clean
clean:
	@for dir in $(all_dirs); do \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: configure
configure:
	@echo "Configuring the customization and configuration directories"
	@for dir in $(all_files); do \
		mkdir -p $$dir; \
	done
	@echo "Complete!"
