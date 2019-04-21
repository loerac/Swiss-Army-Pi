CC=gcc
COMMON_CFLAGS= -Wall -Werror

CFLAGS+=$(EXTRACFLAGS) \
	$(COMMON_CFLAGS)

LINKFLAGS+=$(EXTRALIBS) \
	   $(EXTRACFLAGS) \
           $(LIBS_ENTRY)

.PHONY: all
all: $(TARGET).$(TARGETTYPE)

.PHONY: clean
clean:
	-rm -f *.o *.a *.d

$(TARGET).so: $(OFILES)
	@echo Generating static library $@
	mkdir -p $(LIB_PATH)
	$(CC) $(CFLAGS) -fPIC -c $(SOURCES)
	$(CC) -shared -Wl,-soname,$(TARGET).so -o $(LIB_PATH)/$@ $^

$(TARGET).exe: $(LIBS_PATHS) $(OFILES)
	@echo Main Generate $@
	$(info Binary is Linking against [${OFILES}] and [${LINKFLAGS}])
	mkdir -p $(BIN_PATH)
	$(CC) -o $(BIN_PATH)/$(TARGET) $(OFILES) $(LINKFLAGS)

.c.o:
	@echo C Object Generate $@
	$(CC) $(CFLAGS) -c $*.c
	$(CC) -MM $(CFLAGS) -c $*.c -o $*.d
	@mv -f $*.d $*.d.tmp

	sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	rm -f $*.d.tmp
