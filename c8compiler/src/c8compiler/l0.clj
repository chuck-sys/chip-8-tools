(ns c8compiler.l0)

(defn exp->str
  "Transform base s-expression into string. Does not check for syntax."
  [[op & args]]
  (cond
    (= op 'begin) (clojure.string/join "\n" (map exp->str args))
    (= op 'label) (format "%s:" (first args))
    (= op 'data) (format "%s" (clojure.string/join " " (map str args)))
    :else (format "%s %s"
                  (.toUpperCase (str op))
                  (clojure.string/join ", " (map str args)))))
