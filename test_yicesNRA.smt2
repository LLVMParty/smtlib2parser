; Source: https://github.com/SRI-CSL/yices2?tab=readme-ov-file#non-linear-arithmetic
;; QF_NRA = Quantifier-Free Nonlinear Real Arithmetic
(set-logic QF_NRA)
;; Declare variables
(declare-fun x () Real)
(declare-fun y () Real)
;; Find solution to x^2 + y^2 = 1, x = 2*y, x > 0
(assert (= (+ (* x x) (* y y)) 1))
(assert (= x (* 2 y)))
(assert (> x 0))
;; Check
(check-sat)
;; Get the model
(get-model)