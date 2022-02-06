#lang racket
(require (for-label c8compiler/utilities
                    racket)
         scribble/bettergrammar
         c8compiler/utilities)

(define-grammar
  level1-lang

  #:literals (fvar? int4? int8? int12? int16? symbol?)
  #:datum-literals (begin cls ret jp call se sne ld add sub subn or and xor shr shl rnd drw skp sknp bcd
                          label data i dt st k v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd)
  (p (begin e ...))

  (e (begin e ...)
     (cls) (ret)
     (jp location) (jp reg location)
     (call location)
     (se loc prim)
     (sne loc prim)
     (ld loc prim) (ld i location) (ld loc dt) (ld loc k) (ld dt prim) (ld st prim)
     (add loc prim) (add i loc)
     (sub loc prim) (subn loc prim)
     (or loc prim)
     (and loc prim)
     (xor loc prim)
     (shr loc) (shl loc)
     (rnd loc byte)
     (drw loc loc nibble)
     (skp loc)
     (sknp loc)
     (bcd loc)
     (label fixed-location)
     (data word ...)
     )

  (location fixed-location literal-location)

  (loc reg fvar)

  (prim reg byte fvar)

  (fvar fvar?)

  (token reg byte)

  (reg v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd)

  (word int16?)

  (literal-location int12?)

  (byte int8?)

  (nibble int4?)

  (fixed-location symbol?))

(provide level1-lang)
