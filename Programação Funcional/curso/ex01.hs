--perimetro
perimetro :: Float -> Float
perimetro r = 2*pi*r

--hipotenusa
hipotenusa :: Float -> Float -> Float
hipotenusa a b = sqrt (a*a + b*b)

--diferenca area circulos
diferenca :: Float -> Float -> Float
diferenca r1 r2 = abs(pi*r1*r1 - pi*r2*r2)
