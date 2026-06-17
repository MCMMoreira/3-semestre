#ifndef ARVOREBEST_H
#define ARVOREBEST_H

#include <stdio.h>

#include "funcoes.h"

// #define taman 5000
#define M 4
#define MM (2 * M)
typedef int TipoChave;


typedef enum
{
    Interna,
    Externa
} TipoIntExt; // tipo enumerado

typedef struct TipoPag *TipoAp;

typedef struct TipoPag
{
    TipoIntExt pt; // pt diz se vai ser pagina interna ou externa

    union
    {
        struct
        {
            int ni;
            TipoChave ri[2 * M];
            TipoAp pi[2 * M + 1];
        } interna; // estrutura de uma pagina interna

        struct
        {
            int ne;
            TipoRegistro re[2 * M];
        } externa; // estrutura de uma pagina externa
    } uniao;

} TipoPag;

void Inicializa(TipoAp *Arvore);
int PesquisaBest(Resultados *resultados, TipoRegistro *x, TipoAp *ap);
void CriaArvoreBest(Resultados *resultados, FILE *arquivo, int quantidade, TipoAp *Arvore);
void InsereArv(Resultados *resultados, TipoRegistro registro, TipoAp Ap, short *Cresceu, TipoRegistro *Retorno, TipoAp *ApRetorno);
void InsereReg(Resultados *resultados, TipoRegistro registro, TipoAp *Ap);
void InserePaginaExterna(Resultados *resultados, TipoAp Ap, TipoRegistro Chave);
void LiberaArvoreBest(TipoAp *arvore);
void ImprimirChavesExternas(TipoAp pagina);
#endif
