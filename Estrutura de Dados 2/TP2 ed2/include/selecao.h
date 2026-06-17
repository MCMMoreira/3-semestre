#ifndef SELECAO_H
#define SELECAO_H

#include "funcoes.h"

#include <stdbool.h>
#include <stdio.h>

void heaprefaz(RegistroParaSubstituicao* registros, int n, int i, Resultados* resultados); 
void construirHeap(RegistroParaSubstituicao* registros, int n, Resultados* resultados); 
RegistroParaSubstituicao extrairMin(RegistroParaSubstituicao* registros, int* n, Resultados* resultados); 
bool todosMarcados(RegistroParaSubstituicao* registros, int n);
void desmarcarRegistros(RegistroParaSubstituicao* registros, int n);
void imprimirHeap(RegistroParaSubstituicao* registros, int n); 
void selecaoPorSubstituicao(FILE *arquivoEntrada, int quantidade, Resultados* resultados);

#endif // SELECAO_H
