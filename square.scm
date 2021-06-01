(define (square size)
  (define turn-dir -90)
  (define half-size (/ size 2))
  (tortoise-center)
  (tortoise-penup)
  (tortoise-turn 90)
  (tortoise-move half-size)
  (tortoise-turn 90)
  (tortoise-move half-size)
  (tortoise-turn 180)
  (tortoise-pendown)
  (define (iter i)
	(tortoise-move size)
	(tortoise-turn -90)
	(if (= i 0)
		0
		(iter (- i 1))))
  (iter 3))

(define (many-squares n step)
  (square n)
  (if (<= n 0)
	  0
	  (many-squares (- n step) step)))

(many-squares 15 0.5)

  
