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
  (not (false? (member x '(v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd)))))

(define (prim? x)
  (or (register? x)
      (int8? x)
      (fvar? x)))

(define (token? x)
  (or (register? x)
      (int8? x)))

(define word? int16?)

;; reify fvars into appropriate offsets
(define (implement-fvars p)
  ;; lhs-fvar: loc? (reg? -> s-exp) -> s-exp
  ;; FIXME this is wrong bc not all opcodes are assignments. fix the name.
  (define (lhs-fvar loc cb)
    (if (fvar? loc)
      `(begin
         (ld i ,(+ (current-base-stack-address) (fvar->index loc)))
         (pop v0)
         ,(cb 'v0))
      (cb loc)))

  (match p
    [`(begin ,es ...) `(begin ,@(map implement-fvars es))]
    [`(,se-op ,loc ,prim)
      (lhs-fvar loc (λ (r) `(,se-op ,r ,prim)))]))
