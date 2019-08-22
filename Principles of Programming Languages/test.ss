#lang racket
(define a '(h e l l o))
(+ 2 2)
  (define test '(2 4 5 7 9))
  (define fifth (caddr (cdr test)))
 (define test1 (car (car '((3)))))
 (define test2 (caar '((3))))

(define count
  (lambda (x l)
    (cond
      ((null? l) 0)
      ((eq? x (car l)) (+ 1 (count x ( cdr l))))
      (else (count x (cdr l))))))