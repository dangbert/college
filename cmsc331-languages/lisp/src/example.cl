;; Common LISP Cheat Sheet for CMSC 331 - Nicholas
;; version of November 15, 2016
;;
;; comments start with two semicolons
;;
;; On GL, you can type clisp to invoke common Lisp
;; (load "CommonLispCheatSheet") slurps in this file
;; to create a PDF for this document:
;; enscript -C -Eelisp -2r --media=Letter -o LCS.ps CommonLispCheatSheet.cl
;; ps2pdf LCS.ps LCS.pdf

(defun helloWorld ()
  ;; prints the standard message
  (print "Hey there World!"))

(defun helloUser ()
  (princ "Please type your age:")
  (let ((name (read-line)))
    (princ "Nice to meet you, you young whipper snapper, ")
    (princ name))  
)

;; fun with predicates
(defun predicateDemo ()
  (print (member 3 (list 1 2 3 4 5 6)))
  (print (symbolp 'foo))
  (print (eq 4 (+ 2 2)))  ; good for atoms
  (print (equal '(1 2 3) '(1 2 3)))  ; good for other things
  (print (null ()))  ; should be TRUE
  (print (listp ()))  ; should be TRUE
  (print (listp '(1 2 3)))  ; should be TRUE
  (print (listp '3))  ; should be FALSE
  (print (or (oddp 3) (evenp 3)))  ; should be TRUE
  (print (and (oddp 3) (evenp 3)))  ; should be FALSE
)

;; lisp has global variables...
(defparameter *aGlobalVar* 1)   ; note the earmuffs
(defvar *colors* (list 'red 'green 'blue))
(setf (first *colors*) 'yellow)
(print *colors*)

;; demonstrate the use of a persistent value
;; update this if the file name changes, of course
(defvar *thisFileName* "example")
(defun reload (&optional (fileName *thisFileName*))
  (load fileName))

;; (defun aFunction (possibly empty list of parameters)
;; <function body>
;; ) 

;; ash is arithmetic shift
(defun ashDemo ()
  (print (list (ash 16 2) (ash 8 -1)))
)

;; let creates local variables, e.g.
(defun letDemo ()
  (print (let ((a 6) (b 3)) (+ a b)))
)

;; labels creates local functions that can call each other, e.g.
(defun labelsDemo ()
  (print (labels ((a (n) (+ n 5))
	   (b (n) (+ (a n) 6)))
    (b 10)))
)

;; an example of expt
(defun exptDemo ()
  (print (let ((pi 3.14) (e 2.7)) (list (expt pi e) (expt e pi))))
)

;; basic list manipulation in Common Lisp
(defun listDemo()
  (print (cons 'a 'b)) ; creates a dotted pair
  (print (car '(this is a list)))
  (print (cdr '(this is another list)))
  (print (cons 'quick '(brown fox)))
  (print (reverse '(1 2 3)))
  (print (member 3 (list 1 2 3 4 5 6)))
)

;; of course every language needs an if statement
(defun ifDemo()
  (print (if (= (+ 1 2) 3) 
  	     'yup 
	     'nope
	 )
  )
)

;; Lisp has when and unless, too
(defun whenDemo (aNumber)
  (when (oddp aNumber)
    ;; t is the default output stream, namely the console
    (format t "aNumber is ~a" aNumber)
    (format t "and it is odd") 
  )
)

;; Charles does a reverse
(defun newReverse (aList)
    (if (null aList) 
    	()    ;; then clause
    	(append (newReverse (cdr aList)) (list (car aList)))   ;; else clause
    )
)

;; Charles does a list length thing
(defun newLength (aList)
    (if (null aList) 
        0  ;; then
	(+ 1 (newLength (rest aList)))
    )
)

;; the cond statement does more!
(defun condDemo()
  (let ((a 2))
    (cond ((eq a 1) (princ "a is 1"))
	  ((eq a 3) (princ "a is 3"))
	  (t        (princ "a is something else")))
    )
)

;; creating an association list
(defparameter *nodes* '((living-room (you are in a living room))
			(garden (you are in a garden))
			(attic (you are in the attic))))
(defun assocDemo()
  (print (assoc 'garden *nodes*))
)

;; example of mapcar
(defun mapcarDemo()
;  (mapcar #'sqrt '(1 2 3 4 5))
  (mapcar (function sqrt) '(1 2 3 4 5))
)

(defun itsOne (aThing) 1)  ;; map any argument to 1

(defun mapDemo()
  (let ((a (mapcar #'abs '(3 -4 2 -5 -6)))
        (b (mapcar #'+ '(1 2 3) '(4 5 6))))
       (list a b)
  )
)

;; example of apply
(defun applyDemo()
  (apply #'append '((10 20) (30) (40 50)))
)

(defun newLength2 (aList)  ;; demonstrate mapcar AND apply
       (if (null aList)
           0
           (apply #'+ (mapcar #'itsOne aList))
       )
)

;; examples of reduce
(reduce #'+ '(1 2 3 4 5))
(reduce #'* '(1 2 3 4 5))

;; example of append
(defun appendDemo()
  (append '(1 2) '(3 4))
)

;; an example from Barski
(defun say-hello ()
  (princ "Please type your name:")
  (let ((name (read-line)))
    (princ "Nice to meet you, ")
    (princ name)))

;; explore some features as described at http://common-lisp.net
(defun rationalDemo()
  (print (+ 1/1 1/2 1/3 1/4 1/5 1/6 1/7 1/8 1/9 1/10 1/11 1/12))
  (princ "should be the same as")
  (print (harmonicSum 12))
)

;; sum of the harmonic series
(defun harmonicSum(n)
   (if (= n 1) 
       1  ;; then
       (+ (/ 1 n) (harmonicSum(- n 1)))
   )
)

;; generalize the Lisp + function
(defun sum (aList)
  (if (null aList) 
      0
    (+ (car aList) (sum (rest aList)))
    )
)

(defun sum2 (aList) (reduce #'+ aList))

(defun sum3 (aList)
  (defun helper (aList subtotal)
    (if (null aList)
	subtotal
      (helper (rest aList) (+ (first aList) subtotal))
      )
    )
  (helper aList 0)
)

;; which is faster, sum, sum2, or sum3?
;; generate a list of random integers, and time each of them

(defun random-list (n maxval)
  (mapcar #'random (make-list n :initial-element maxval )))

(defun timeTrial (n maxval)
  (let ((aList (random-list n maxval)))
    (when (< n 1000)
      (princ "ordinary recursion")
      (time (sum aList)))
    (when (< n 1000)
      (princ "tail recursion")
      (time (sum3 aList)))
    (princ "using mapcar")
    (time (sum2 aList))
    )
)

;; use this function to run all the others
(defun allDemos()
  (when t
    (helloWorld) (predicateDemo) (listDemo)
    (ashDemo) (letDemo) (labelsDemo) (exptDemo) (ifDemo) 
    (condDemo) (assocDemo) (mapcarDemo) (appendDemo) 
    (applyDemo) (print (newReverse '(5 4 3 2 1)))
    (print (newLength '(5 4 3 2 1))) (rationalDemo)
    t)
)
