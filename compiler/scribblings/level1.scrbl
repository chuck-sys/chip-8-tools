#lang scribble/manual
@require[@for-label[compiler
                    racket]
         scribble/bettergrammar
         "level0.rkt"
         "level1.rkt"]

@title{Level 1: Getting the most out of our "stack"}

Usually with this pass we try to abstract away registers in favour of locations in memory. But since only
@tt{I} register is 16-bit (and thus can address all program memory), it is impossible to effectively
even have frame registers (there is no opcode that stores a 16-bit number into memory, nor any opcode to
perform 16-bit arithmetic @bold{effectively}, so we can't reuse @tt{I}). Thus the entire program
will only have 256 variables, plus a few select registers. This shouldn't be a huge problem since in later
passes we will try to cut down on register use by checking if reuse is possible.

We store variables in an upwards growing stack that starts at @code{I = #xF00}, and every time we want a
variable, we do:

@codeblock{
; variable at offset + 12
LD I, #xF0C         ; #xF00 (stack start) + #x00C (offset)
POP V0
; just for ref, this is how we store variables
LD I, #xF0C
PUSH V0
}

Which will store the 8-bit value in @tt{V0}. This also means that our programs cannot meaningfully make use
of @tt{V0}, since we would freqently need it to store variables. This also means that our programs cannot be
larger than @code{#xFFF - #x200 - #x0FF = #xD00 = 3328} bytes.

@bettergrammar*-diff[level0-lang level1-lang]

Next, we'd want to abstract away both types of registers (both registers and offsets). We'll call these
objects @italic{abstract locations}. Because of scope and shadowing issues, abstract locations are symbols
in the format @code{<name>.<number>}. Because of optimization concerns, we'd want to use as few stack
variables as possible, since it makes our code inefficient (adds at least 4 bytes to some instruction), so
we will try to assign abstract locations to registers if possible, and only when impossible should we
assign them to frame variables.

At this point, we have successfully removed all mentions of both registers and memory from our language.
