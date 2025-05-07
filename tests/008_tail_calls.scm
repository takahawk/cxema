(define more-than-stack 100000)
(define (if-check x) 
  (if (> x 0) 
    (if-check (- x 1)) 
    0))
(if-check more-than-stack)

(define (cond-check x)
  (cond ((= x 0) 0)
        (else (cond-check (- x 1)))

(cond-check more-than-stack)


(define (even? n)
  (if (= n 0) #t (odd? (- n 1))))

(define (odd? n)
  (if (= n 0) #f (even? (- n 1))))

(even? more-than-stack) 
