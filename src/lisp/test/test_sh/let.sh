#./scheme_test "(let)"
#./scheme_test "(let . t)"
#./scheme_test "(let t t)"
#./scheme_test "(let (x))"
#./scheme_test "(let (x) . t)"
#./scheme_test "(let (x 3) x)"
#./scheme_test "(let (x nil) x)"
#./scheme_test "(let (x t) x)"
#./scheme_test "(let ((x . 3)) x)"

./scheme_test "(let (x) x)"
./scheme_test "(let ((x)) x)"
./scheme_test "(let ((x 3)) x)"
./scheme_test "(let ((x 3)(y 4)) x)"
#./scheme_test "(let ((x 3)(x 4)) x)" <-sbcl ではエラー clisp は OK
./scheme_test "(let ((x 3)(y 4)) y)"
./scheme_test "(let ((x 3)(y 4)) (defun xy () y))"
