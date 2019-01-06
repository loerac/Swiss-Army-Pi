BIN := $(shell pwd)/bin
SRC := src

.PHONY: directories

all:
	mkdir -p $(BIN)
	$(MAKE) -C $(SRC) $@

clean:
	rm -rf $(BIN)
	$(MAKE) -C $(SRC) $@
