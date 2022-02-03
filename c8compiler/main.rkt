#lang racket/base

(module+ test
  (require rackunit))

;; Notice
;; To install (from within the package directory):
;;   $ raco pkg install
;; To install (once uploaded to pkgs.racket-lang.org):
;;   $ raco pkg install <<name>>
;; To uninstall:
;;   $ raco pkg remove <<name>>
;; To view documentation:
;;   $ raco docs <<name>>
;;
;; For your convenience, we have included LICENSE-MIT and LICENSE-APACHE files.
;; If you would prefer to use a different license, replace those files with the
;; desired license.
;;
;; Some users like to add a `private/` directory, place auxiliary files there,
;; and require them in `main.rkt`.
;;
;; See the current version of the racket style guide here:
;; http://docs.racket-lang.org/style/index.html

;; Code here
(require
  racket/match
  racket/string

)

; sexp->string: s-expression -> string
(define (sexp->string sexp)
  ; arg->string: (or/c number? symbol?) -> string
  (define (arg->string arg)
    (if (number? arg)
      (number->string arg)
      (symbol->string arg)))

  (match sexp
    [`(begin ,exps ...) (string-join (map sexp->string exps) "\n")]
    [`(label ,l) (format "~a:" (symbol->string l))]
    [`(data ,nums ...) (string-join (map number->string nums))]
    [`(,op) (string-upcase (symbol->string op))]
    [`(,op ,args ...) (format "~a ~a"
                              (string-upcase (symbol->string op))
                              (string-join (map arg->string args) ", "))]))

(define compile (compose sexp->string))


(module+ test
  ;; Any code in this `test` submodule runs when this file is run using DrRacket
  ;; or with `raco test`. The code here does not run when this file is
  ;; required by another module.

  (check-equal? (sexp->string '(begin (label sprite))) "sprite:")

  (check-equal? (sexp->string '(begin (begin (begin (label sprite))))) "sprite:"))

(module+ main
  ;; (Optional) main submodule. Put code here if you need it to be executed when
  ;; this file is run using DrRacket or the `racket` executable.  The code here
  ;; does not run when this file is required by another module. Documentation:
  ;; http://docs.racket-lang.org/guide/Module_Syntax.html#%28part._main-and-test%29

  (require racket/cmdline)
  (define out (box (current-output-port)))
  (command-line
    #:program "c8-compiler"
    #:once-each
    [("-o" "--output") fn "Which file to output assembly to" (set-box! out (open-output-file fn))]
    #:args (input-file)
    (let* ([in (open-input-file input-file)]
           [output (compile (read in))])
      (display output (unbox out)))))
