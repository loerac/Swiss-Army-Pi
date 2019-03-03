
CC=gcc
CFLAGS+=$(EXTRACFLAGS)

LINKFLAGS+=$(EXTRALIBS) \
          $(LIBS_ENTRY)

COMMON_CFLAGS= -Wall -Werror
-include $(OFILES:.o=.d)

ifdef DEPS_DIR
CFLAGS += -I$(DEPS_DIR)/include
endif

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	-rm -f *.o *.a *.d

$(TARGET).object: $(OFILES)

$(TARGET): $(LIBS_PATHS) $(OFILES)
	@echo Generate $@
	$(info Binary is Linking against [${OFILES}] and [${LIBS_PATHS}])
	mkdir -p $(BIN_PATH)
	$(CC) -o $(BIN_PATH)/$(TARGET) $(OFILES) $(LINKFLAGS)
	@sleep 1

.c.o:
	@echo Generate $@
	$(CC) $(CFLAGS) -c $*.c
	$(CC) -MM $(CFLAGS) -c $*.c -o $*.d
	@mv -f $*.d $*.d.tmp

	sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	rm -f $*.d.tmp
