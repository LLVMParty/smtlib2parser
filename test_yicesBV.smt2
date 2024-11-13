; Source: https://github.com/SRI-CSL/yices2?tab=readme-ov-file#bit-vectors
;; QF_BV = Quantifier-Free Bit-Vectors
(set-logic QF_BV)
;; Declare variables
(declare-fun x () (_ BitVec 32))
(declare-fun y () (_ BitVec 32))
;; Find solution to (signed) x > 0, y > 0, x + y < x
(assert (bvsgt x #x00000000))
(assert (bvsgt y #x00000000))
(assert (bvslt (bvadd x y) x))
;; Check
(check-sat)
;; Get the model
(get-model)