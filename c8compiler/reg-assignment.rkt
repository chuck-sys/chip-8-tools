#lang racket
(require "utilities.rkt")

(define (location? x)
  (or (fixed-location? x)
      (literal-location? x)))

(define fixed-location? symbol?)

(define literal-location? int12?)

(define (loc? x)
  (or (register? x)
      (fvar? x)))

(define (register? x)
  (not (false? (member x '(v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd ve)))))

(define (prim? x)
  (or (register? x)
      (int8? x)
      (fvar? x)))

(define (token? x)
  (or (register? x)
      (int8? x)))

(define word? int16?)
