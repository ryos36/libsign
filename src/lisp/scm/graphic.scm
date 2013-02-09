(define (draw-line xy0 xy1 color)
	(+ 1 color))

(define (color r g b)
	(+ r g b))

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
	  (make-segment (make-vect  65  85) (make-vect  60 100))
	  (make-segment (make-vect  35  85) (make-vect  40  65))
	  (make-segment (make-vect  50  30) (make-vect  40  00)))))

(define wave
  (segments->painter
    (list (make-segment (make-vect 35 85) (make-vect 40 100))
	  (make-segment (make-vect  65  85) (make-vect  60 100))
	  (make-segment (make-vect  35  85) (make-vect  40  65))
	  (make-segment (make-vect  65  85) (make-vect  60  65))
	  (make-segment (make-vect  60  65) (make-vect  75  65))
	  (make-segment (make-vect  40  65) (make-vect  30  65))
	  (make-segment (make-vect  75  65) (make-vect 100  35))
	  (make-segment (make-vect  60  45) (make-vect 100  15))
	  (make-segment (make-vect  60  45) (make-vect  75  00))
	  (make-segment (make-vect  50  30) (make-vect  60  00))
	  (make-segment (make-vect  30  65) (make-vect  15  60))
	  (make-segment (make-vect  30  60) (make-vect  15  40))
	  (make-segment (make-vect  15  60) (make-vect  00  85))
	  (make-segment (make-vect  15  40) (make-vect  00  65))
	  (make-segment (make-vect  30  60) (make-vect  35  50))
	  (make-segment (make-vect  35  50) (make-vect  25  00))
	  (make-segment (make-vect  50  30) (make-vect  40  00)))))

(define wave
  (segments->painter
    (list (make-segment (make-vect 0.35 0.85) (make-vect 0.40 1.00))
          (make-segment (make-vect 0.65 0.85) (make-vect 0.60 1.00))
          (make-segment (make-vect 0.35 0.85) (make-vect 0.40 0.65))
          (make-segment (make-vect 0.65 0.85) (make-vect 0.60 0.65))
          (make-segment (make-vect 0.60 0.65) (make-vect 0.75 0.65))
          (make-segment (make-vect 0.40 0.65) (make-vect 0.30 0.65))
          (make-segment (make-vect 0.75 0.65) (make-vect 1.00 0.35))
          (make-segment (make-vect 0.60 0.45) (make-vect 1.00 0.15))
          (make-segment (make-vect 0.60 0.45) (make-vect 0.75 0.00))
          (make-segment (make-vect 0.50 0.30) (make-vect 0.60 0.00))
          (make-segment (make-vect 0.30 0.65) (make-vect 0.15 0.60))
          (make-segment (make-vect 0.30 0.60) (make-vect 0.15 0.40))
          (make-segment (make-vect 0.15 0.60) (make-vect 0.00 0.85))
          (make-segment (make-vect 0.15 0.40) (make-vect 0.00 0.65))
          (make-segment (make-vect 0.30 0.60) (make-vect 0.35 0.50))
          (make-segment (make-vect 0.35 0.50) (make-vect 0.25 0.00))
          (make-segment (make-vect 0.50 0.30) (make-vect 0.40 0.00)))))

(define f (make-frame '(100 100) '(-100 0) '(0 -100)))
;(define f (make-frame '(100 100) '(80 50) '(20 40)))
(wave f)
wave
