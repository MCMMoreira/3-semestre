#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "funcoes.h"

#define MEM_EXT_QUICK 10

typedef Aluno TipoArea[MEM_EXT_QUICK];

void QuickSortExterno (FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir, Resultados *resultados);

void LeSup(FILE **ArqLEs, Aluno* UltLido, int *Ls, short *OndeLer, Resultados *resultados);

void LeInf(FILE **ArqLi, Aluno* UltLido, int *Li, short *OndeLer, Resultados *resultados);

void InserirArea (TipoArea Area, Aluno* UltLido, int *NRArea, Resultados* resultados);

void EscreveMax (FILE **ArqLEs, Aluno R, int *Es, Resultados *resultados);

void EscreveMin (FILE **ArqEi, Aluno R, int *Ei, Resultados *resultados);

void RetiraMax(TipoArea Area, Aluno* R, int *NRArea);

void RetiraMin(TipoArea Area, Aluno* R, int *NRArea);

void Particao (FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j, Resultados *resultados);

void FAVazia (TipoArea Area);

void InsereItem (Aluno* UltLido, TipoArea Area, Resultados* resultados);

void RetiraUltimo (Aluno* UltLido, TipoArea Area);

void RetiraPrimeiro (Aluno* UltLido, TipoArea Area);

int ObterNumCelOcupadas (TipoArea Area);

#endif
