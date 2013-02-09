(define func
  (lambda (y0 y1)
    (do ((i y0 (+ i 1))
         ((dot (list 128 i))))
         ((= i y1) 22))))
