#lang scribble/manual
@require[@for-label[c8compiler/utilities
                    racket]
         c8compiler/utilities
         scribble/eval]

@(define sb (make-base-eval '(require c8compiler/utilities)))

@title{Useful utilities}
@defmodule[c8compiler/utilities]

@defproc[(fvar? (x any/c)) boolean?]{
    Return true if @racket[x] is a frame variable. Return false otherwise.

    @examples[
        #:eval sb
        (fvar? 'fv0)
        (fvar? 'fv.0)
        (fvar? 'fv.12)
        (fvar? 'fv.255)
        (fvar? 'fv.-1)
    ]
}

@defproc[(fvar->index (fv fvar?)) integer?]{
    Return the index portion of a frame variable.

    @examples[
        #:eval sb
        (fvar->index 'fv.0)
        (fvar->index 'fv.12)
    ]
}

@defproc[(make-fvar (i (and/c integer? (not/c negative?)))) fvar?]{
    Return a frame variable with the given index.

    @examples[
        #:eval sb
        (make-fvar 0)
        (make-fvar 55)
    ]
}

@defproc*[([(int4? (x any/c)) boolean?]
           [(int8? (x any/c)) boolean?]
           [(int12? (x any/c)) boolean?]
           [(int16? (x any/c)) boolean?])]{
    Return true if @racket[x] is an integer with 4, 8, 12, or 16 bytes. Since Racket is a high-level
    language, we don't know the underlying implementation, so we only check that it is an integer and that
    the value is within the allowed for integers with 4, 8, 12, or 16 bytes. Only considers unsigned
    numbers, since we can't do signed arithmetic.

    @examples[
        #:eval sb
        (int4? 0)
        (int8? 0)
        (int8? #xff)
        (int8? #x100)
        (int12? #xff)
        (int16? #xffff)
    ]
}
