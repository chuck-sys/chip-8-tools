help:
	@echo "make docs			Build the docs"
	@echo "make help			View this help message"

all: help

docs: build/.done

build/.done: scribblings/*.scrbl Makefile
	raco scribble --dest-name build +m --htmls scribblings/compiler.scrbl
	touch $@
