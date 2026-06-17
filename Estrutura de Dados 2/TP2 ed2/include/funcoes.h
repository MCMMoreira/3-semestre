#ifndef FUNCOES_H
#define FUNCOES_H

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define RED(text) "\033[31m" text "\033[0m"
#define GREEN(text) "\033[32m" text "\033[0m"
#define YELLOW(text) "\033[33m" text "\033[0m"

typedef struct{
    long int inscricao;
    double nota;
    char estado [3];
    char cidade [51];
    char curso [31];
}Aluno;

//geracao de blocos por substituicao
typedef struct {

    Aluno registro;
    bool marcado; 

} RegistroParaSubstituicao;

enum TipoFita {ENTRADA = 1, SAIDA = 2};

typedef struct{
    FILE * arq;
    enum TipoFita tipo;
}Fita;

typedef struct{
    long int comparacoes;
    long int leitura;
    long int escrita;
}Quantidades;

typedef struct {

    clock_t tempoOrdenacao[2], tempoSelecao[2];

    Quantidades ordenacao;
    Quantidades selecao;

    double tempoOrdenacaoCalculado;
    double tempoSelecaoCalculado; 

}Resultados;

void transfereArquivo (FILE *arq, int situacao, int quantidade);
void calcularTempo(Resultados *resultado);
void calcularTempoSelecao(Resultados *resultado);
void imprimirResultados(Resultados *resultados);
void imprimirResultadosSelecao(Resultados *resultados);
FILE* abrirArquivo(int situacao, char* modoAbertura, int tipo);

#endif

