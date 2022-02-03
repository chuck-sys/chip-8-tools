#lang racket
(require scribble/bettergrammar
         c8compiler/utilities)

(define-grammar
  level0-lang

  #:literals (int4? int8? int12? int16? symbol?)
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
     (rnd reg byte)
     (drw reg reg nibble)
     (skp reg)
     (sknp reg)
     (bcd reg)
     (push reg)
     (pop reg)
     (label fixed-location)
     (data word ...)
     )

  (location fixed-location literal-location)

  (token reg byte)

  (reg v0 v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd ve)

  (word int16?)

  (literal-location int12?)

  (byte int8?)

  (nibble int4?)

  (fixed-location symbol?))

(provide level0-lang)
