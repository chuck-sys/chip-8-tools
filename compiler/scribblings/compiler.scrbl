#lang scribble/manual
@require[@for-label[compiler
                    racket]]

@title{Chip-8 Compiler (and compiler passes documentation)}
@author{csys}

A compiler that turns S-expressions into assembly. To be used with the assembler included in the chip-8
tools.

Written in Racket because I didnt' want to fight Clojure to get it working. Things such as
@racket[quote-syntax] and @racket[match] that make writing languages so much easier in Racket are absent
in Clojure, and if not absent, then severely lacking:

@racketblock[
(code:comment "racket quote-syntax")
(define something 12)
(define embedded `(something anything 3 ,something))
(code:comment "equivalent in clojure would be `(~'something ~'anything 3 ~something),")
(code:comment "which is just ugly, because of namespacing issues")
]

@include-section["passes.scrbl"]
