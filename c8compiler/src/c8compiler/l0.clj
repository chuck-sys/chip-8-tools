(ns c8compiler.l0)

(defmulti exp->str "Transform base s-expression into string. Does not check for syntax." first)

(defmethod exp->str 'begin
  [[_ & xs]]
  (clojure.string/join "\n" (map exp->str xs)))

(defmethod exp->str 'cls
  [_]
  "CLS")

(defmethod exp->str 'ret
  [_]
  "RET")

(defmethod exp->str 'jp
  [[_ a b]]
  (if (nil? b)
    ; eg: jp start
    ; eg: jp 0x200 (the compiler takes numbers via stoul)
    (str "JP " a)
    ; eg: jp v0, start
    (str "JP " a ", " b)))

(defmethod exp->str 'label
  [[_ l]]
  (str l ":"))
