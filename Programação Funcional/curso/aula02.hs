maior :: Int -> Int -> Int
maior a b = if a >= b then a else b

fatorial :: Int -> Int 
fatorial n 
    | n == 0 = 1
    | n > 0 =  n * fatorial (n-1)

maiornum :: Int -> Int -> Int
maiornum a b 
    | a > b = a
    | a < b = b
    | otherwise = 0