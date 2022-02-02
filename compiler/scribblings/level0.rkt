#lang racket
(require scribble/bettergrammar)

(define-grammar
  level0-lang

  #:literals (integer? symbol?)
  #:datum-literals (begin cls ret jp call se sne ld add sub subn or and xor shr shl rnd drw skp sknp bcd
                          push pop label data i dt st k v0 v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd ve)
  (p (begin e ...))

  (e (begin e ...)
     (cls) (ret)
     (jp location) (jp reg location)
     (call location)
     (se reg token)
     (sne reg token)
     (ld reg token) (ld i token) (ld reg dt) (ld reg k) (ld dt reg) (ld st reg)
     (add reg token) (add i reg)
     (sub reg reg) (subn reg reg)
     (or reg reg)
     (and reg reg)
     (xor reg reg)
     (shr reg)
     (shl reg)
     (rnd reg literal)
     (drw reg reg literal)
     (skp reg)
     (sknp reg)
     (bcd reg)
     (push reg)
     (pop reg)
     (label fixed-location)
     (data literal ...)
     )

  (location fixed-location literal)

  (token reg literal)

  (reg v0 v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd ve)

  (literal integer?)

  (fixed-location symbol?))

(provide level0-lang)
