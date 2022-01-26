(ns c8compiler.core
  (:gen-class)
  (:require [c8compiler.l0 :refer [exp->str]]))

(defn c8->asm
  "Compile lisp chip-8 language into a string of assembly instructions (with newlines). See docs for the whole chip-8 language specifications and examples. This is what's more commonly know as a compiler pipeline."
  [sexp]
  (->> sexp
       (exp->str)))

(defn -main
  "I don't do a whole lot ... yet."
  [& args]
  (let [code (clojure.edn/read)]
    (println (c8->asm code))))
