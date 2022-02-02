#lang info
(define collection "compiler")
(define deps '("base"))
(define build-deps '("scribble-lib"
                     "racket-doc"
                     "rackunit-lib"
                     "scribble-bettergrammar-lib"))
(define scribblings '(("scribblings/compiler.scrbl" ())))
(define pkg-desc "A compiler from a lisp-like language into chip-8 assembly.")
(define version "0.1.0")
(define pkg-authors '(csys))
(define license '(see-parent))
