; Source: https://github.com/SRI-CSL/yices2?tab=readme-ov-file#linear-real-arithmetic
;; QF_LRA = Quantifier-Free Linear Real Arithmetic
(set-logic QF_LRA)
;; Declare variables x, y
(declare-fun x () Real)
(declare-fun y () Real)
;; Find solution to (x + y > 0), ((x < 0) || (y < 0))
(assert (> (+ x y) 0))
(assert (or (< x 0) (< y 0)))
;; Run a satisfiability check
(check-sat)
;; Print the model
(get-model)