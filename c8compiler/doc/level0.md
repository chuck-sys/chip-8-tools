# Level 0 pass

```
p ::= (begin e ...)
e ::= (begin e ...)
    | (cls)
    | (ret)
    | (jp location)
    | (jp reg location)
    | (call location)
    | (se reg token)
    | (sne reg token)
    | (ld reg token)
    | (ld i token)
    | (ld reg dt)
    | (ld reg k)
    | (ld dt reg)
    | (ld st reg)
    | (add reg token)
    | (add i reg)
    | (sub reg reg)
    | (subn reg reg)
    | (or reg reg)
    | (and reg reg)
    | (xor reg reg)
    | (shr reg)
    | (shl reg)
    | (rnd reg literal)
    | (drw reg reg literal)
    | (skp reg)
    | (sknp reg)
    | (bcd reg)
    | (push reg)
    | (pop reg)
    | (label fixed-location)
    | (data literal ...)

location ::= fixed-location
           | literal

token ::= reg
        | literal

reg ::= v0 | v1 | v2 | v3 | v4 | v5 | v6 | v7 | v8 | v9
      | va | vb | vc | vd | ve

literal ::= number?

fixed-location ::= label?
```

In this pass, we remove the last register `VF` since nobody is supposed to be using it. We also implement
expression nesting here. Since this is the lowest level, we don't even have to do checking for syntax
issues. Which is why this pass is the simplest to code.
