#lang scribble/manual
@require[@for-label[c8compiler/utilities
                    racket]
         scribble/bettergrammar
         c8compiler/utilities
         "level0.rkt"
         "level1.rkt"]

@title{Abstracting registers and memory, kinda}
@section{Getting the most out of our "stack"}

Usually with this pass we try to abstract away registers in favour of locations in memory. But since only
@tt{I} register is 16-bit (and thus can address all program memory), it is impossible to effectively
even have frame registers (there is no opcode that stores a 16-bit number into memory, nor any opcode to
perform 16-bit arithmetic @bold{effectively}, so we can't reuse @tt{I}). Thus the entire program
will only have 256 variables, plus a few select registers. This shouldn't be a huge problem since in later
passes we will try to cut down on register use by checking if reuse is possible.

We store variables in an upwards growing stack that starts at @code{I = #xF00}, and every time we want a
variable, we do:

@margin-note{
    We have conveniently placed the compiler function @racket[(current-base-stack-address)] in
    @racket[c8compiler/utilities], which just returns @racket[#xf00].
}

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
larger than @code{#xFFF - #x200 - #x0FF = #xD00 = 3328} bytes. Since we want instructions to be able to
use both places for offsets, and because some instructions must have these two offsets existing at the
same time (for example, the skip-equals and skip-not-equals instructions, or any of the load
instructions), other than @tt{V0}, we must also use another register to store the value of the second
offset. So for instance, these

@codeblock{
(begin
   (se fv.0 fv.1))

(begin
   (add fv.0 fv.1))
}

Would turn into these

@codeblock{
(begin
   (ld i #xf01)
   (pop v0)
   (ld ve v0)
   (ld i #xf00)
   (pop v0)
   (ld v0 v0)          ; useless instruction, removed in some optimization step
   (se v0 ve))

(begin
   (ld i #xf01)
   (pop v0)
   (ld ve v0)
   (add v0 ve)
   (ld i #xf00)
   (push v0))
}

The biggest difference is that instructions that manipulate memory have @tt{push} codes and ones that read
memory have @tt{pop} codes. This also means we are able to remove these from the language to simplify.

@bettergrammar*-diff[level0-lang level1-lang]

We write all offset forms as @code{fv.x}, where @code{x} is an offset and @code{fv} is a symbol. The whole
thing is a symbol, and is supposed to make assigning memory locations and registers a bit easier.

@section{Assigning registers and memory locations to abstract locations}

Next, we'd want to abstract away both types of registers (both registers and offsets). We'll call these
objects @italic{abstract locations}. Because of scope and shadowing issues, abstract locations are symbols
in the format @code{<name>.<number>}. Because of optimization concerns, we'd want to use as few stack
variables as possible, since it makes our code inefficient (adds at least 4 bytes to some instruction), so
we will try to assign abstract locations to registers if possible, and only when impossible should we
assign them to frame variables.

At this point, we have successfully removed all mentions of both registers and memory from our language.
