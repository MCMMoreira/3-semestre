areaheron a b c = sqrt(s * (s - a) * (s - b) * (s - c))
    where
        s = (a+b+c)/2


bhaskara a b c 
    |delta == 0 = 1
    |delta > 0 = 2
    |delta < 0 = 0

    where 
        delta = b*b - 4*a*c


areaheronlet a b c = let s = (a+b+c)/2  
                     in sqrt(s * (s - a) * (s - b) * (s - c))
     