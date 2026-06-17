[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/C81z4oWG)
# Tarefa: Vendas

Uma feirante está vendendo frutas com a seguinte tabela de preços:

| | até 5 kg | acima de 5 kg |
| -- | -- | -- |
| morango | R$ 8,50 por kg | R$ 7,20 por kg |
| maçã | R$ 5,25 por kg | R$ 4,75 por kg |

Defina uma função `valorTotal :: Double -> Double -> Double` que recebe como argumentos as quantidade de morangos e de maçãs adquiridas pelo cliente, e resulta no valor total a ser pago pelo cliente, de acordo com a tabela acima.

Defina a ação de entrada e saída `main :: IO ()` que, quando executada, obtém da entrada padrão a quantidade (em kg) de morangos e a quantidade (em kg) de maçãs adquiridos pelo cliente e, caso esses dados sejam válidos (não negativos), insere na saída padrão o valor a ser pago pelo cliente, que deve ser calculado usando a função `valorTotal`. Caso algum dado seja negativo, a mensagem _Entrada inválida_ deve ser inserida na saída padrão.

Exemplos de execução do programa:
```shellsession
Digite a quantidade de Morangos (em kg): -15
Digite a quantidade de Maçãs (em kg): 8
Entrada inválida
```
```shellsession
Digite a quantidade de Morangos (em kg): 10
Digite a quantidade de Maçãs (em kg): −1
Entrada inválida
```
```shellsession
Digite a quantidade de morangos (em kg): 0
Digite a quantidade de macas (em kg): 10.5
O valor total da compra é R$ 49.875
```
```shellsession
Digite a quantidade de Morangos (em kg): 0
Digite a quantidade de Maçãs (em kg): 0
O valor total da compra é R$ 0.00
```
```shellsession
Digite a quantidade de morangos (em kg): 10.8
Digite a quantidade de macas (em kg): 5
O valor total da compra é R$ 104.01
```

Complete as definições em `src/*.hs` para resolver o problema proposto. O seu projeto deve passar em todos os testes fornecidos.

Você pode editar o arquivos fonte em um editor de texto, e testá-lo diretamente no ambiente interativo, ou com a ferramenta _cabal_ em um terminal:

``` shellsession
$ cabal build             # compila o projeto
$ cabal repl vendas       # carrega o projeto no ambiente interativo
$ cabal run vendas        # executa o programa
$ cabal test              # executa os testes
```
