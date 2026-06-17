module Main (main) where

import Test.Hspec
import Test.Hspec.QuickCheck
import Test.QuickCheck

import Run
import qualified Vendas as T

main :: IO ()
main = hspec $ do
  describe "Custo das vendas" $ do
    it "zero" $ do
      T.valorTotal 0 0 `shouldBe` 0
    prop "cálculo 1" $ do
      forAll (choose (0, 5)) $ \x ->
        forAll (choose (0, 5)) $ \y ->
          T.valorTotal (x :: Double) (y :: Double) `shouldBe` 8.5*x + 5.25*y
    prop "cálculo 2" $ do
      forAll (choose (6, 10)) $ \x ->
        forAll (choose (0, 5)) $ \y ->
          T.valorTotal (x :: Double) (y :: Double) `shouldBe` 7.2*x + 5.25*y
    prop "cálculo 3" $ do
      forAll (choose (0, 5)) $ \x ->
        forAll (choose (6, 100)) $ \y ->
          T.valorTotal (x :: Double) (y :: Double) `shouldBe` 8.5*x + 4.75*y
    prop "cálculo 4" $ do
      forAll (choose (6, 100)) $ \x ->
        forAll (choose (6, 100)) $ \y ->
          T.valorTotal (x :: Double) (y :: Double) `shouldBe` 7.20*x + 4.75*y
  describe "Executando o programa" $ do
    it "com entradas inválidas 1" $ do
      out <- runWithInput T.main (unlines (map show [-10, 20]))
      shouldContain out "Entrada inválida"
    it "com entradas inválidas 2" $ do
      out <- runWithInput T.main (unlines (map show [10, -20]))
      shouldContain out "Entrada inválida"
    it "com entradas válidas" $ do
      out <- runWithInput T.main (unlines (map show [10.8, 5]))
      shouldContain out "valor total da compra é R$ 104.01"
