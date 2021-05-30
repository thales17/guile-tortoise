(define (draw-ploygon! circumference vertices)
  (let ((side (/ circumference vertices))
		(angle (/ 360.0 vertices)))
	(let iterate ((i 1))
	  (if (<= i vertices)
		  (begin
			(tortoise-move side)
			(tortoise-turn angle)
			(iterate (1+ i)))))))

(draw-ploygon! 16 4)

(tortoise-penup)
(tortoise-move 1)
(tortoise-turn 30)
(tortoise-pendown)
(draw-ploygon! 12 3)

(tortoise-penup)
(tortoise-move -2)
(tortoise-turn -100)
(tortoise-pendown)
(draw-ploygon! 10 64)
