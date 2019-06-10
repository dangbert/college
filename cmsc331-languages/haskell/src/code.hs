{-
Notes on Haskell.
To load into ghci:

ghci
:load code
main (to run the main function)

:r   (to reload the code)
:?   (for help)
:t   (to see the type of an object)
:q   (to quit)

-- NOTES:
-- http://yannesposito.com/Scratch/en/blog/Haskell-the-Hard-Way/
-- https://www.youtube.com/watch?v=02_H3LjqMr8
--
-- once a value is set it can't be changed (immutable)
--
-- head returns the first element of a list
-- tail returns all elements in a list except the first
--
-- http://learnyouahaskell.com/types-and-typeclasses
-- Function parameters are separated with -> and there's no special distinction
-- between the parameters and the return type. (The return type is the last item
-- in the declaration)
-}

import qualified Data.Char as Char
import Data.Ratio      -- let's you use rational numbers
import Data.List       -- let's us invoke sort later


{-
-- Decides if a value is an element of the list (without using elem function).
-- everything before the => is called a class constraint.
-- The class constraint (Eq a) ensures that the elements of type a belong to
-- the eq class (and thus can be tested for equality)
--
-- a and [a] are the function parameters and Bool is the return type
-- the first parameter is the value were looking for
-- the second parameter is the array we're looking in
-}
myElm :: Eq a => a -> [a] -> Bool
myElm _ [] = False     -- underscore means the first parameter can be any value
                       -- so it matches to any value followed by an empty array.
myElm target (x:xs) =  -- Binds the head of the list to x, and the rest to xs
    if x == target then True -- check if the head of the list is what we're searching for
    else myElm target xs     -- recursively continue searching the rest of the list


{-
-- Produces a list of identical elements.
--
-- TODO: use a higher order function instead
-- (a high order function is one that takes functions as parameters and/or
-- returns functions as return values)
-- like list comprehension
-}
myReplicate :: Int -> a -> [a]
myReplicate num value =
    take num (repeat value)
    -- replicate num value -- also does the same thing


{-
-- Splits an array into two halves.
-- returns a tuple of the two halves
-- if the array has an odd number of elements, then the first half with be 1 longer
-}
halve :: [a] -> ([ a], [a])
halve arr =
    -- define local variables
    let len = length arr
        -- integer quotient of len and 2 truncated towards 0,
        -- then 1 is added if the array has an odd number of elements
        middle = (quot len 2) + mod len 2
    -- use the variables in an expression (in is not needed when the let is in a do block)
    in splitAt middle arr -- splits arr in half and returns a tuple of the two halves


{-
-- returns the list of all pythagorean triples whose components are at most a given limit
-- for example: pyths 10 => [(3,4,5),(4,3,5),(6,8,10),(8,6,10)]
-- https://youtu.be/02_H3LjqMr8?t=1260
-}
pyths :: Int -> [(Int, Int, Int)]
pyths n =
    -- create an array of 3-tuples such that each value is between 1 and n
    -- (inclusive) and a^2 + b^2 == c^2 (they're pythagorean triples)
    [(a,b,c) | a <- [1..n], b <- [1..n], c <- [1..n], a^2 + b^2 == c^2]


{-
-- Recursively merges two sorted lists to give a single sorted list.
-}
merge :: Ord a => [a] -> [a] -> [a]
merge list1 [] = list1
merge [] list2 = list2

-- the @ symbol is read as "as". It binds the first list to list1,
-- and the first element of list1 to x, and the rest of list1 to xs
merge list1@(x:xs) list2@(y:ys) | x <= y  =
    x : merge xs list2
merge list1@(x:xs) list2@(y:ys) | x > y  =
    y : merge list1 ys


{-
-- Implements the merge sort algorithm.
-- in mergesort, the empty list and singleton lists are already sorted, and any
-- other list is sorted by merging together the two lists that result from
-- applying mergesort to the two halves of the list separately.
-}
msort :: Ord a => [a] -> [a]
msort [] = []
msort arr | length arr == 1  = arr

-- everything after the $ operator takes precedence over anything that comes before
-- the . operator chains functions: f . g x <=> f (g x)
-- fst gets the first element of a tuple, and snd gets the second
msort arr = merge (msort . fst $ halve arr) (msort . snd $ halve arr)


{-
-- Test all the functions above.
-- must be all IO actions
-}
main = do
    putStrLn ("myElm 7 [0,1,2,3,4,6,8,9,10,77] => " ++ show(myElm 7 [0,1,2,3,4,6,8,9,10,77]))
    putStrLn ("myElm 6 [0,1,2,3,4,6,8,9,10,77] => " ++ show(myElm 6 [0,1,2,3,4,6,8,9,10,77]))
    putStrLn("")

    putStrLn ("myReplicate 9 7 => " ++ show(myReplicate 9 7))
    putStrLn ("myReplicate 4 'a' => " ++ show(myReplicate 4 'a'))
    putStrLn("")

    putStrLn ("halve [5] => " ++ show(halve [5]))
    putStrLn ("halve [1,2,3,4,5,6,7,8,9,10] => " ++ show(halve [1,2,3,4,5,6,7,8,9,10]))
    putStrLn ("halve [0,44,6,9,0,12,55] => " ++ show(halve [0,44,6,9,0,12,55]))
    putStrLn("")

    putStrLn ("pyths 4 => " ++ show(pyths 4))
    putStrLn ("pyths 12 => " ++ show(pyths 12))
    putStrLn ("pyths 15 => " ++ show(pyths 15))
    putStrLn("")

    putStrLn ("merge ['g','i','o','z'] ['f','h','k','p'] => " ++ show(merge ['g','i','o','z'] ['f','h','k','p']))
    putStrLn ("merge [0,2,5,8,15,18,22] [-2,3,4,7,16,25,33,100] => " ++ show(merge [0,2,5,8,15,18,22] [-2,3,4,7,16,25,33,100]))
    putStrLn("")


    putStrLn ("msort [7,5,1,6,2,3] => " ++ show(msort [7,5,1,6,2,3]))
    putStrLn ("msort [-3,5,-4,7,-12,89] => " ++ show(msort [-3,5,-4,7,-12,89]))
    putStrLn ("msort ['y','i','o','z', 'p'] => " ++ show(msort ['y','i','o','z', 'p']))
    putStrLn("")
