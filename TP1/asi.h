#ifndef ASI_H
#define ASI_H

#include <stdbool.h>
#include <stdio.h>

#include "funcoes.h"

#define TAM 5000
#define ITENSPAGINA_ASI 20
#define MAXTABELA 15000

// Acesso Sequencial Indexado
int geraIndice(Resultados *resultados, FILE *arq, TipoRegistro *tabela);
bool pesquisa(Resultados *, TipoRegistro *tab, int tam, TipoRegistro *item, FILE *arq, int situacao);
bool buscaItem(Resultados *, TipoRegistro pagina[ITENSPAGINA_ASI], TipoRegistro *item, int situacao);

#endif
