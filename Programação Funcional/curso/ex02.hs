ehpar :: Int -> Bool
ehpar x = if mod x 2 == 0 then True else False

charcase :: Char -> String
charcase ch | ch >= 'a' && ch <= 'z' = "Minúsculo"
            | ch >= 'A' && ch <= 'Z' =  "Maiúsculo"
            | otherwise = "Desconhecido                                                                                                                                                                                      "


