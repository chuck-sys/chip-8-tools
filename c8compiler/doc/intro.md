# Introduction to c8compiler

The c8compiler is a compiler that compiles higher-level S-expressions into the lower-level assembly
instructions, which can then be fed into the assembler to be turned into Chip 8 machine code. Here is an
example program:

```lisp
(program
  (let ((ball (rect 2 2)))
    (loop ((x (/ WIDTH 2))
           (y (/ HEIGHT 2))
           (vx 1)
           (vy 1))
      (cls)
      (draw x y ball)

      (recur
        (+ x vx)
        (+ y vy)
        (if (or (zero? x) (= x WIDTH))
          (- 0 vx)
          vx)
        (if (or (zero? y) (= y HEIGHT))
          (- 0 vy)
          vy)))))
```

Since Chip 8 has no opcode for division, it can either be handled by inserting a function that
specifically does this, or the compiler can optimize it out. In this case, since both arguments are
constants, we can have the compiler do the work for us. Both `WIDTH` and `HEIGHT` are known constants, and
are inserted during compilation. Built-in functions (such as calls to the division function) are only used
if needed to decrease the amount of code generated.

This language has no variables, and instead uses bindings, which makes everything immutable.

# c8compiler passes

1. Level 0
