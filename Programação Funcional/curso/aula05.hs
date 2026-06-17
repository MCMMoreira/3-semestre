compr [] = 0
compr (h:t) = 1 + compr t

soma [] = 0
soma (h:t) = h + soma t

encontraChar :: [Char] -> Char -> Bool
encontraChar [] ch = False
encontraChar (h:t) ch
    |h == ch = True
    |otherwise = encontraChar t ch

encontraMaior [] m = 0
encontraMaior (h:t) m
    |null t == True = m
    |h>m = encontraMaior t h
    |otherwise = encontraMaior t m

maiore1 [] = -1
maiore1 (h:t) = if h >= maiorcauda then h else maiorcauda
    where
        maiorcauda = maiore1 t

raizes a b c |delta < 0 = []
             |delta == 0 = [(-b)/(2*a)]
             |delta > 0 = [(-b - sqrt delta)/(2*a), (-b + sqrt delta)/(2*a)]

        where
            delta = b*b - 4*a*c
        
multiplos n = [x*n| x <- [1..10]]

ehprimo n = null [x | x <- [2, 3..n-1], mod n x == 0]