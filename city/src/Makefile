.PHONY: all

.PHONY: clean
clean:
	rm -rf ${BIN_PATH}
	rm -rf ${LIB_PATH}
	${MAKE} -C service $@

.PHONY: city
city:
	${MAKE} -C service all
