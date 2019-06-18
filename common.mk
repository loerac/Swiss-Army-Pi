CC	= gcc
MKDIR	= mkdir -p
CFLAGS	= -fPIC -Wall -Werror
CFLAGS  += $(EXTRA_CFLAGS)
#LDFLAGS = -shared
#LDFLAGS += $(EXTRA_LDFLAGS)
RM	= rm -rf
CP	= cp

PREFIX 	?= /usr/local
EXPORT_LIB = $(PREFIX)/lib
EXPORT_INC = $(PREFIX)/include

LIB_OBJECT := $(subst .c,.o,$(SOURCE))
LIB_DEPEND := $(subst .o,.d,$(LIB_OBJECT))

# Link files
BUILD = src lib
all: $(BUILD)

.PHONY: all $(BUILD)
src: $(TARGET).$(TYPE)

lib: $(TARGET).$(TYPE)

$(TARGET).exe: $(LIB_OBJECT)
	$(MKDIR) $(OUTPUT_BIN)
	@echo Generate $@
	$(CC) -o $(OUTPUT_BIN)/$(TARGET) $^ $(EXTRA_LDFLAGS)

$(TARGET).so: $(LIB_OBJECT)
	$(MKDIR) $(OUTPUT_LIB)
	$(MKDIR) $(OUTPUT_INC)
	$(CC) $(CFLAGS) -c $(SOURCE)
	$(CC) -shared -Wl,-soname,$(TARGET).$(TYPE) -o $(OUTPUT_LIB)/$(TARGET).$(TYPE) $^
	$(CP) -t $(OUTPUT_INC) -ulf $(addprefix $(CURDIR)/,$(LIB_INCLUDE))
	@echo

# Pull in dependency info for existing .o files
-include $(LIB_DEPEND)

.PHONY: install
install:
	$(CP) -a $(OUTPUT_LIB)/$(TARGET).$(TYPE) $(EXPORT_LIB)
	$(CP) -t $(EXPORT_INC) $(addprefix $(OUTPUT_INC)/,$(LIB_INCLUDE))
	@echo

.PHONY: install_data
install_data:
	-$(CP) -t $(TARGET_CUST) -a $(addprefix $(OUTPUT_DATA)/,$(TARGET_DATA_CUST))
	-$(CP) -t $(TARGET_CONF) -a $(addprefix $(OUTPUT_DATA)/,$(TARGET_DATA_CONF))
	@echo

.PHONY: uninstall
uninstall:
	$(RM) $(EXPORT_LIB)/$(TARGET).$(TYPE)
	$(RM) $(addprefix $(EXPORT_INC)/,$(LIB_INCLUDE))
	@echo

.PHONY: uninstall_data
uninstall_data:
	$(RM) $(addprefix $(TARGET_CUST)/,$(TARGET_DATA_CUST))
	$(RM) $(addprefix $(TARGET_CONF)/,$(TARGET_DATA_CONF))
	@echo

# Compile and generate dependency info
# More complicated dependency computation, so all pre-reqs
# listed will also become command-less, pre-reqs-less targets
#	sed: 	strip the target (everything before the colon)
#	sed: 	remove any continuation backslashes
#	fmt -1: list words one per line
#	sed: 	strip leading spaces
#	sed:	add trailing colons
%.o: %.c
	@echo
	@echo "Generating object $(@)"
	$(CC) -c $(CFLAGS) $*.c -o $*.o
	$(CC) -MM $(CFLAGS) $*.c >  $*.d
	@mv -f $*.d $*.d.tmp
	sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	$(RM) $*.d.tmp
	@echo

# Compile and generate dependency info
.c.o:
	@echo "Generate binary $(TARGET).$(TYPE)"
	$(CC) $(CFLAGS) -c $*.c
	$(CC) -MM $(CFLAGS) -c $*.c -o $*.d
	@mv -f $*.d $*.d.tmp
	sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	$(RM) $*.d.tmp

.PHONY: clean
clean:
	-$(RM) $(OUTPUT_BIN) $(OUTPUT_LIB) $(OUTPUT_INC) $(LIB_OBJECT) $(LIB_DEPEND)
	@echo
