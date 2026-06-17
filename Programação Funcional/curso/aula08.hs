maior a b = if (a > b) then True else False

menor a b = if (a < b) then True else False
 
 buscalista :: (Int->Int->Bool)->[Int]->Int buscalista _ [] = -1
 buscalista _ (cabeca:[]) = cabeca
 buscalista f (cabeca:cauda) = if (f cabeca x) then cabeca else x
    where
        x = buscalista f cauda