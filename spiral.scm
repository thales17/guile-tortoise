(define (spiral)
  (define (iter a r)
	(tortoise-center)
	(tortoise-turn a)
	(tortoise-penup)
	(tortoise-move r)
	(tortoise-turn 90)
	(tortoise-pendown)
	(tortoise-move 0.01)
	(if (= a 0)
		0
		(iter (- a 1) (+ r 0.001))))
  (iter (* 25 360) 0.01))
(spiral)
