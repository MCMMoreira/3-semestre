#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdbool.h>
#include <stdio.h>

#include <string.h>
#include <time.h>

#include "funcoes.h"

// CORES    
#define RED(text) "\033[31m" text "\033[0m"
#define GREEN(text) "\033[32m" text "\033[0m"
#define YELLOW(text) "\033[33m" text "\033[0m"

// ESTRUTURAS

// Estrutura para armazenar os registros da arvore B

#define tamanho 5000

typedef struct TipoRegistro {
    int Chave;
    long int Dado1;
    char Dado2[tamanho];
} TipoRegistro;

// Estrutura para armazenar os registros da arvore B*

#define taman 5000

typedef struct TipoReg
{
    int chave;
    long int dado1;
    char dado2[taman];
} TipoReg;

// Estrutura para armazenar os resultados obtidos

typedef struct{
    int comparacoes;
    int transferencias;
}Quantidades;

typedef struct Resultados{

    clock_t tempoPreProcessamento[2], tempoPesquisa[2];
    
    TipoRegistro pesquisar;
    TipoReg pesquisarEstrela;

    Quantidades preProcessamento;
    Quantidades pesquisa;

    double tempoPreProcessamentoCalculado;
    double tempoPesquisaCalculado;
    double tempoTotalCalculado;
    
} Resultados;

// FUNCOES
bool inputsValidados(int metodo, int quantidade, int situacao);
void calcularTempo(Resultados *resultado);
bool gerarNumerosAleatorios(char * nomeArquivo, int quantidade, Resultados * resultados);
bool verificaChaveRepetida(int posicaoRand, int *posicao, int n);
void imprimirResultados(Resultados *resultados, int quantidadeExecucoes);
void imprimirChavesArquivo(char *nomeArquivo, int quantidade);


#endif
