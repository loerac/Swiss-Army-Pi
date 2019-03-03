.PHONY: all

.PHONY: clean
clean:
	rm -rf ${BIN_PATH}
	rm -rf ${LIB_PATH}
	${MAKE} -C service $@

.PHONY: wifi
wifi:
	${MAKE} -C service all
