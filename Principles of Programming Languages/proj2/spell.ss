
; *********************************************
; *  314 Principles of Programming Languages  *
; *  Spring 2017                                      *
; *  Student Version                                 *
; * Anirudh Tunoori                                  *
; * netid: at813                                      *
; *********************************************

;; contains "ctv", "A", and "reduce" definitions
(load "include.ss")

;; contains simple dictionary definition
(load "dictionary.ss")

;; -----------------------------------------------------
;; HELPER FUNCTIONS

;; *** CODE FOR ANY HELPER FUNCTION GOES HERE ***

;; -------------------------------------------------------
;;UNUSED HELPER FUNCTIONS
;; Get the lest element in a list (used iin key to read letters right to left)
;;DIDN'T USE
;(define (last_let l)
 ; (cond ((null? (cdr l)) (car l))
  ;     (else (last_let (cdr l))))
  ;)
;scrap function
;(define key_helper
 ; (lambda (w)
  ;    (cond ((null? w) 5187)
   ;     (else (+ (* 29 ( key( reverse (cdr (reverse w))) )    (ctv (last_let w)    ) )
    ;           
     ;   )))
    ;))

;;A function that checks if an element can be found in a list
;;it is used in conjunction with comp
;(define (member? x list)
 ;    (cond ((null? list) #f)
  ;         ( (equal? x (car list)) #t)
   ;        (else   (member? x (cdr list)))))

;;A function that essentially determines if the "wordvector" is  a subset of the bitvector
;;Used to check if a word (once hashed) can be found in the bitvector
;(define comp
 ;    (lambda (l ll)
  ;     (cond ((null? ll) #t)
   ;;         ((not (equal? #t (member? (car ll) l ))) #f)
     ;       (else (comp l (cdr ll)) ) 
       ;     ) 
      ; ))
;; tests:
;(define l '(30 1 0))
;(define ll '(3 6))
;(comp l ll)

;;_____________________________________________
;;USED HELPER FUNCTIONS

;;flatten fucntion; taken from hw6
;;used to "un-nest" the vector
(define flatten
  (lambda (l)
    (cond ((null? l) '())
    ((pair? l)
     (append(flatten (car l)) (flatten (cdr l))))
    (else (list l)) )
 ))

;;A function that determines if l1 is a subset of l2
(define (subset? l1 l2)
  (or (null? l1)
      (and (member  (car l1) l2)
           (subset? (cdr l1) l2)))
  )

;;A function that essentially determines if the "wordvector" is  a subset of the bitvector
;;Used to check if a word (once hashed) can be found in the bitvector
(define (comp l1 l2)
  (and (subset? l1 l2)))

;;This function generates a bitvector that is used by the spell checker generator
;;The bitvector is created once for each spell checker
(define gen_bitVector
  (lambda (hashfunctionlist dict bitvector)
     (cond ((null? dict) '())
      (else 
       (flatten (map (lambda (word) (cons (map (lambda (hashfunction) (hashfunction word) ) hashfunctionlist ) bitvector) ) dict ))
       ))
    ))

;;This function generates a list of hash values obtained by applying the hash functions
;;found in the hashfunction list onto the word. This list is returned as a "wordvector" and its
;;contents are compared with the bitvector in order to determine if the word can be found in the dictionary
(define gen_wordVector
  (lambda (hashfunctionlist wordvector x)
     (cond ((null? hashfunctionlist) '() )
          (else
             (flatten (map (lambda (word) (cons (map (lambda (hashfunction) (hashfunction word))
                               hashfunctionlist) wordvector)) (list x)))
        ))
    ))

;; -----------------------------------------------------
;; KEY FUNCTION

(define key         ;5187 is the initial value; for each character (right to left) key is multipllied by 29 + ctv(c)
  (lambda (w)
    (reduce (lambda (c k) (+ (* 29 k) (ctv c))) w 5187)
    ))

;; -----------------------------------------------------
;; EXAMPLE KEY VALUES
 ;(key '(h e l l o))         = 106402241991
 ;(key '(m a y))            = 126526810
 ;(key '(t r e e f r o g)) = 2594908189083745

;; -----------------------------------------------------
;; HASH FUNCTION GENERATORS

;; value of parameter "size" should be a prime number
(define gen-hash-division-method
  (lambda (size) ;; range of values: 0..size-1
    (lambda (k)  ;; h(k) = k mod size
       (modulo (key k) size)
       )
))

;; value of parameter "size" is not critical
;; Note: hash functions may return integer values in "real"
;;       format, e.g., 17.0 for 17
(define gen-hash-multiplication-method
  (lambda (size) ;; range of values: 0..size-1
    (lambda(k)   ;; h(k) = ⌊size ∗ (k ∗ A - ⌊k ∗ A⌋)⌋
      (floor (* size ( - (* (key k) A) (floor (* (key k) A) ) ) ))
      )
))

;; -----------------------------------------------------
;; EXAMPLE HASH FUNCTIONS AND HASH FUNCTION LISTS

(define hash-1 (gen-hash-division-method 70111))
(define hash-2 (gen-hash-division-method 89997))
(define hash-3 (gen-hash-multiplication-method 7224))
(define hash-4 (gen-hash-multiplication-method 900))

(define hashfl-1 (list hash-1 hash-2 hash-3 hash-4))
(define hashfl-2 (list hash-1 hash-3))
(define hashfl-3 (list hash-2 hash-3))

;; -----------------------------------------------------
;; EXAMPLE HASH VALUES
;;   to test your hash function implementation
;;
;; (hash-1 '(h e l l o))           ==> 35616
;;  (hash-1 '(m a y))             ==> 46566
;;  (hash-1 '(t r e e f r o g))  ==> 48238
;;
;;  (hash-2 '(h e l l o))       ==> 48849
;;  (hash-2 '(m a y))           ==> 81025
;;  (hash-2 '(t r e e f r o g)) ==> 16708
;;
;;  (hash-3 '(h e l l o))       ==> 6331.0
;;  (hash-3 '(m a y))           ==> 2456.0
;;  (hash-3 '(t r e e f r o g)) ==> 1806.0
;;
;;  (hash-4 '(h e l l o))       ==> 788.0
;;  (hash-4 '(m a y))           ==> 306.0
;;  (hash-4 '(t r e e f r o g)) ==> 225.0

;; -----------------------------------------------------
;; SPELL CHECKER GENERATOR

;;Generates a spell checker when given adictionary, a list of hash functions, and a word
;;The bitvector is only generated once.
;;Once generated, to determine if the given word can be found in the dictionary, the function checks if the
;;"wordvector" is a subset of the bitvector
(define gen-checker
  (lambda (hashfunctionlist dict)
    (lambda (word)
      (let* ( (bitvector '()) (wordvector '()) (l1 '()) (l2 '()) )
        (let * ( (l2 (gen_bitVector hashfunctionlist dict bitvector))
                 (l1 (gen_wordVector hashfunctionlist wordvector word))) 
          (comp l1 l2)))
      )))

;; -----------------------------------------------------
;; EXAMPLE SPELL CHECKERS

(define checker-1 (gen-checker hashfl-1 dictionary))
(define checker-2 (gen-checker hashfl-2 dictionary))
(define checker-3 (gen-checker hashfl-3 dictionary))

;(printf "~a: " (gen_bitVector hashfl-1 dictionary '()))
;(gen_wordVector hashfl-1 '() '(h e l l o))
;(comp (gen_bitVector hashfl-1 dictionary '()) (gen_wordVector hashfl-1 '() '(h e l l o)))
;; EXAMPLE APPLICATIONS OF A SPELL CHECKER
;;
;; (checker-1 '(a r g g g g)) ==> #f
;; (checker-2 '(h e l l o)) ==> #t
;; (checker-2 '(a r g g g g)) ==> #t  ;; false positive

