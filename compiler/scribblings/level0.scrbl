#lang scribble/manual
@require[@for-label[compiler
                    racket]
         scribble/bettergrammar
         "level0.rkt"]

@title{Level 0: From S-expressions to Strings}

@bettergrammar*[level0-lang]

In this pass, we remove the last register @tt{VF} since nobody is supposed to be using it. We also
implement expression nesting here. Since this is the lowest level, we don't even have to do checking for
syntax issues. Which is why this pass is the simplest to code.
