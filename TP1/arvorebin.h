#ifndef ARVOREBIN_H
#define ARVOREBIN_H

#include <stdio.h>
#include <stdbool.h>

#include "funcoes.h"

#define TAM 5000
#define ITENSPAGINA 20

typedef struct{
    int esq;
    TipoRegistro item;
    int dir;
    int pos; //posicao no arquivo binario
}Nodo;

FILE* constroiArvore (Resultados *resultados,FILE *arquivo);
bool insereNodo (Resultados *resultados,Nodo novo, FILE* arvore, int numNodos);
bool pesquisaItem (Resultados *resultados,TipoRegistro *item, Nodo pagina[ITENSPAGINA], Nodo aux, int *posRet);

#endif
