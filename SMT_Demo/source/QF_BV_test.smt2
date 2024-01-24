; SMT language demo
; QF_BV Quantifier_Free Bit Vector Theroy

(set-logic QF_BV)

(declare-fun x () (_ BitVec 8))
(declare-fun y () (_ BitVec 8))
(declare-fun z () (_ BitVec 8))

(assert (= (bvxor x (bvand y z)) #b00001100)) ; x ^ (y & z) == 12
(assert (= (bvand y (bvlshr z #b00000011)) #b00000011)) ; (y & (z >> 3)) == 3
(assert (= (bvxor z y) #b00000100)) ; z ^ y == 4
