#lang racket

(define (current-base-stack-address) #xf00)

(define (fvar? x)
  (and (symbol? x)
       (match (string-split (symbol->string x) ".")
         [`(,a ,b) (and (string=? a "fv")
                        (integer? (string->number b))
                        (not (negative? (string->number b))))]
         [else #f])))

(define (fvar->index fv)
  (match-let ([`(,_ ,i) (string-split (symbol->string fv) ".")])
    (string->number i)))

(define (make-fvar i)
  (string->symbol (format "fv.~a" i)))

(define (int4? x)
  (and (integer? x)
       (<= 0 x #xf)))

(define (int8? x)
  (and (integer? x)
       (<= 0 x #xff)))

(define (int12? x)
  (and (integer? x)
       (<= 0 x #xfff)))

(define (int16? x)
  (and (integer? x)
       (<= 0 x #xffff)))

(provide (contract-out
           [fvar? (-> any/c boolean?)]
           [fvar->index (-> fvar? integer?)]
           [make-fvar (-> (and/c integer? (not/c negative?)) fvar?)]

           [int4? (-> any/c boolean?)]
           [int8? (-> any/c boolean?)]
           [int12? (-> any/c boolean?)]
           [int16? (-> any/c boolean?)]

           [current-base-stack-address (-> int12?)]
           ))

(module+ test
  (require rackunit)

  (check-true (fvar? 'fv.0))
  (check-true (fvar? 'fv.2))
  (check-false (fvar? 'fv.-1))
  (check-false (fvar? 'fv0))

  (check-equal? (fvar->index 'fv.0) 0)
  (check-equal? (fvar->index 'fv.12) 12)

  (check-equal? (make-fvar 0) 'fv.0)
  (check-equal? (make-fvar 12) 'fv.12)

  (check-true (int4? 0))
  (check-true (int8? 0))
  (check-true (int12? 0))
  (check-true (int16? 0))
  (check-true (int4? #xf))
  (check-true (int8? #xff))
  (check-true (int12? #xfff))
  (check-true (int16? #xffff))
  (check-false (int4? #x10))
  (check-false (int8? #x100))
  (check-false (int12? #x1000))
  (check-false (int16? #x10000))
  )


