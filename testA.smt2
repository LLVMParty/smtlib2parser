(set-logic QF_UF)
(declare-fun p () Bool)
(assert (and p (not p)))
(check-sat)