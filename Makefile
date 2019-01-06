src_city := city
src_wifi := wifi
sources  := $(src_city) $(src_wifi)

.PHONY: all $(sources)
all:
	$(MAKE) -C $(src_city) $@
	$(MAKE) -C $(src_wifi) $@

clean:
	$(MAKE) -C $(src_city) $@
	$(MAKE) -C $(src_wifi) $@
