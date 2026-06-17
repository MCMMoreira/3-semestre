#ifndef ARVOREB_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "funcoes.h"


#define tamanho 5000
#define M 4
#define MM (2 * M)

typedef struct TipoPagina *TipoApontador;

typedef struct TipoPagina {
    int n;
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
} TipoPagina;

void Inicializar(TipoApontador *Arvore);
bool Pesquisa(Resultados *resultados,TipoRegistro *registro, TipoApontador Ap, int chave);
void InsereArvore(Resultados *resultados,TipoRegistro registro, TipoApontador Ap, bool *Cresceu, TipoRegistro *RegistroRetorno, TipoApontador *ApRetorno);
void InsereRegistro(Resultados *resultados,TipoRegistro registro, TipoApontador *Ap);
void InserePagina(Resultados *resultados,TipoApontador Ap, TipoRegistro Registro, TipoApontador ApDir);
void CriaArvore(Resultados *resultados,FILE *arquivo, int quantidade, TipoApontador *Arvore);
void liberarArvoreB(TipoApontador Arvore);

#endif
