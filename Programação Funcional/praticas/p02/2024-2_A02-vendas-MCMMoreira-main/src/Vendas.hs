module Vendas where

import System.IO (stdout, hSetBuffering, BufferMode(NoBuffering))
import Distribution.PackageDescription.FieldGrammar (validateBenchmark)

valorTotal :: Double -> Double -> Double
valorTotal morangos maçãs = valMo + valMa
    where
        valMo = if morangos <= 5
                    then morangos * 8.5
                    else morangos * 7.2
        
        valMa = if maçãs <= 5
                    then maçãs * 5.25
                    else maçãs * 4.75
main :: IO ()
main =
    do 
    hSetBuffering stdout NoBuffering

    putStr "Digite a quantidade de Morangos (em kg): "
    pesoMorango <- readLn

    putStr "Digite a quantidade de Maçãs (em kg): "
    pesoMacas <- readLn

    if pesoMorango < 0 || pesoMacas < 0
        then putStrLn "Entrada inválida"
        else putStrLn ("O valor total da compra é R$ " ++ show (valorTotal pesoMorango pesoMacas))


