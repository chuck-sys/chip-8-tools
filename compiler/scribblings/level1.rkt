#lang racket
(require scribble/bettergrammar)

(define-grammar
  level1-lang

  #:literals (integer? symbol?)
  #:datum-literals (begin cls ret jp call se sne ld add sub subn or and xor shr shl rnd drw skp sknp bcd
                          push pop label data i dt st k + v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd ve)
  (p (begin e ...))

  (e (begin e ...)
     (cls) (ret)
     (jp location) (jp reg location)
     (call location)
     (se reg token)
     (sne reg token)
     (ld loc prim) (ld i token) (ld loc dt) (ld reg k) (ld dt prim) (ld st prim)
     (add loc prim) (add i reg)
     (sub loc prim) (subn loc prim)
     (or loc prim)
     (and loc prim)
     (xor loc prim)
     (shr loc)
     (shl loc)
     (rnd loc literal)
     (drw loc loc literal)
     (skp reg)
     (sknp reg)
     (bcd reg)
     (push reg)
     (pop reg)
     (label fixed-location)
     (data literal ...)
     )

  (location fixed-location literal)

  (loc reg offset)

  (prim reg literal offset)

  (offset (ve + literal))

  (token reg literal)

  (reg v1 v2 v3 v4 v5 v6 v7 v8 v9 va vb vc vd ve)

  (literal integer?)

  (fixed-location symbol?)
  )

(provide level1-lang)
