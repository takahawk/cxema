(define (make-adder n)
  (lambda (x) (+ x n)))

(define add5 (make-adder 5))
(add5 3) ; 8
((make-adder 1000) 337) ; 1337

(define (outer)
  (define (inner)
    1337)
  inner)

((outer)) ; 1337
