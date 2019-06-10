;;;; Daniel Engbert CMSC 331-01 Fall 2016
;;;; various lisp functions
;;;;
;;;; commands to run:
;;;; clisp
;;;; (load "code.cl")
;;;; (sphereVolume 3)


;;; computes the volume of a sphere given its radius
(defun sphereVolume(r)
    (print "The volume of the sphere is ")
    (* (/ 4 3) 3.14159265359 (expt r 3))
)


;;; computes the real and complex roots of a quadratic equation
;;; ax^2 + bx + c
;;; and returns them as a list
;;; note: complex numbers like 1+2i are represented like: #C(1 2) in lisp
(defun roots(a b c)
    ;; let* allows variables in the let to depend on each other
    (let*
        ;; define the local variables d, r1, and r2:
        (
            (d (- (expt b 2) (* 4 a c)) )
            (r1
                (/
                    (+ (- b) (sqrt d))
                    (* 2 a)
                )
            )
            (r2
                (/
                    (- (- b) (sqrt d))
                    (* 2 a)
                )
            )
        )
        ;; now run the code below:

        ;; t is the standard output
        ;; ~3$ means print the variable to 3 decimal places
        ;; ~% is a new line
        ;;(format t "roots: ~3$, ~3$~%" r1 r2) ; prints the results

        ;; return a list of the roots
        (if (= r1 r2)
            (list r1)     ; when the roots are equal (i.e. 0)
            (list r1 r2)  ; else when the roots are different
        )
    )
)


;;; takes a simple list of numbers and return a list consisting of the smallest
;;; and largest numbers in the set
(defun getBounds(nums)
    ;; the apply function passes all elements in nums as arguments to the function that's passed as a symbol
    ;; apostrophe is shorthand for the quote function which passes the min function as a symbol
    (list (apply 'min nums) (apply 'max nums)) ; return a list containing the min and max value
)

;;; implements the quicksort algorithm by sorting a list of integers
(defun  quickSort(nums)

)


;;; returns the number of elements in aList that fall in the range min <= x <=max
;;; where x is an element of aList.
;;; If aList is empty, the function should return 0
;;; You can define additional “helper” functions if you like.
(defun rangeCount(aList minVal maxVal)
    (between 0 aList minVal maxVal)  ; call between and return its result
)


;; recursive helper function for rangeCount
;; total is the running total of elements in the original list that fall in the range
;; note:
;; car gets the first item in the list
;; cdr returns the list without the first item
(defun between(total aList minVal maxVal)
    ;; TODO: when (car aList) is null, return total)
    ;; consider using cond function
    (if
        ;; condition: minVal <= (car aList) <= maxVal
        (and
            (<= minVal (car aList))
            (<= (car aList) maxVal)
        )
        ;; then increment total and call between and return its results
        (between (+ total 1) (cdr aList) minVal maxVal)
        ;; else call between and return its result
        (between total (cdr aList) minVal maxVal)
    )
)
