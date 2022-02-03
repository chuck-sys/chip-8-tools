help:
	@echo "make docs			Build the docs"
	@echo "make help			View this help message"

all: help

docs: build

build: scribblings/* Makefile
	raco scribble --dest-name build +m --redirect-main https://docs.racket-lang.org/ --htmls scribblings/compiler.scrbl
