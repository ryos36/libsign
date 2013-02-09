(define (make-vect x y) 
	(list x y))

(define (xcor-vect xy)
	(car xy))

(define (ycor-vect xy)
	(cadr xy))

(define (scale-vect s xy)
	(make-vect 
		(* s (xcor-vect xy))
		(* s (ycor-vect xy))))

(define (add-vect xy0 xy1)
	(make-vect 
		(+ (xcor-vect xy0) (xcor-vect xy1))
		(+ (ycor-vect xy0) (ycor-vect xy1))))

(define (make-frame o-xy e1-xy e2-xy)
	(list o-xy e1-xy e2-xy))

(define (origin-frame frame)
	(car frame))

(define (edge1-frame frame)
	(cadr frame))

(define (edge2-frame frame)
	(caddr frame))

(define (frame-coord-map frame)
	(lambda (v)
		(add-vect
			(origin-frame frame)
			(add-vect (scale-vect (xcor-vect v)
					      (edge1-frame frame))
			          (scale-vect (ycor-vect v)
					      (edge2-frame frame)
					      (edge1-frame frame))))))

(define (make-segment s-xy e-sy)
	(list s-xy e-sy))

(define (start-segment seg)
	(car seg))

(define (end-segment seg)
	(cadr seg))

;(define (draw-line xy0 xy1 color)
;	(+ 1 color))
	
(define (segments->painter segment-list)
  (lambda (frame)
    (map
      (lambda (segemnt)
        (draw-line ((frame-coord-map frame) (start-segment segemnt))
                   ((frame-coord-map frame) (end-segment segemnt))
		   (color 0 255 0)))
       segment-list)))

(define wave
  (segments->painter
      (list (make-segment (make-vect 35 85) (make-vect 40 100))
                (make-segment (make-vect 65 85) (make-vect 60 100)))))

(define f (make-frame '(0 0) '(0 100) '(100 0)))
(wave f)
wave
