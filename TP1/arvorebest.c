#include <stdio.h>
#include "arvorebest.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int PesquisaBest(Resultados *resultados, TipoRegistro *x, TipoAp *ap)
{
    if (ap == NULL || *ap == NULL)
    {
        // Ponteiro nulo, não há página para buscar
        return 0;
    }

    TipoAp pag = *ap;

    if (pag->pt == Interna)
    {
        int i = 0;

        // Busca na página interna

        resultados->pesquisa.comparacoes += 1;
        while (i < pag->uniao.interna.ni && x->Chave > pag->uniao.interna.ri[i])
        {
            i++;
            resultados->pesquisa.comparacoes += 1;
        }

        // Recursivamente pesquisa na próxima página
        return PesquisaBest(resultados, x, &pag->uniao.interna.pi[i]);
    }
    else
    {
        // Página externa
        int i = 0;

        // Busca na página externa
        resultados->pesquisa.comparacoes += 1;
        while (i < pag->uniao.externa.ne && x->Chave > pag->uniao.externa.re[i].Chave)
        {
            i++;
            resultados->pesquisa.comparacoes += 1;
        }

        // Verifica se encontrou a chave
        resultados->pesquisa.comparacoes += 1;

        if (i < pag->uniao.externa.ne && x->Chave == pag->uniao.externa.re[i].Chave)
        {
            // Copia os dados do registro encontrado para o registro passado
            x->Chave = pag->uniao.externa.re[i].Chave;
            x->Dado1 = pag->uniao.externa.re[i].Dado1;
            strncpy(x->Dado2, pag->uniao.externa.re[i].Dado2, taman);
            x->Dado2[taman - 1] = '\0'; // Garante que a string seja terminada corretamente
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

void Inicializa(TipoAp *Arvore)
{
    *Arvore = NULL;
}

void InserePaginaExterna(Resultados *resultados, TipoAp Ap, TipoRegistro Registro)
{
    int k = Ap->uniao.externa.ne - 1;

    // Encontra a posição correta para inserir a chave
    resultados->preProcessamento.comparacoes += 1;

    while (k >= 0 && Registro.Chave < Ap->uniao.externa.re[k].Chave)
    {
        Ap->uniao.externa.re[k + 1] = Ap->uniao.externa.re[k];
        k--;

        resultados->preProcessamento.comparacoes += 1;
    }

    // Insere a chave na posição encontrada
    Ap->uniao.externa.re[k + 1] = Registro;

    // Incrementa o número de elementos na página externa
    Ap->uniao.externa.ne++;
}

void InsereArv(Resultados *resultados, TipoRegistro registro, TipoAp Ap, short *Cresceu, TipoRegistro *Retorno, TipoAp *ApRetorno)
{
    int i = 0;
    TipoAp ApTemp;
    TipoChave chaveMeio;

    if (Ap == NULL)
    {
        *Cresceu = 1;
        *Retorno = registro;
        *ApRetorno = NULL;
        return;
    }

    // Página externa
    if (Ap->pt == Externa)
    {

        if (Ap->uniao.externa.ne < M * 2)
        {
            InserePaginaExterna(resultados, Ap, registro);
            *Cresceu = 0;
            return;
        }

        // Divisão de página externa
        ApTemp = (TipoAp)malloc(sizeof(TipoPag));
        if (!ApTemp)
        {
            printf("Erro de alocação de memória!\n");
            exit(EXIT_FAILURE);
        }

        ApTemp->pt = Externa;
        int l = 0;

        for (int j = M; j < M * 2; j++)
        {
            ApTemp->uniao.externa.re[l++] = Ap->uniao.externa.re[j];
        }

        ApTemp->uniao.externa.ne = l;
        Ap->uniao.externa.ne = M;

        chaveMeio = Ap->uniao.externa.re[M - 1].Chave;

        resultados->preProcessamento.comparacoes++;

        if (registro.Chave < chaveMeio)
        {
            InserePaginaExterna(resultados, Ap, registro);
        }
        else
        {
            InserePaginaExterna(resultados, ApTemp, registro);
        }

        *Cresceu = 1;
        Retorno->Chave = chaveMeio;
        *ApRetorno = ApTemp;
        return;
    }

    // Página interna
    resultados->preProcessamento.comparacoes += 1;
    while (i < Ap->uniao.interna.ni && registro.Chave > Ap->uniao.interna.ri[i])
    {
        i++;
        resultados->preProcessamento.comparacoes += 1;
    }

    InsereArv(resultados, registro, Ap->uniao.interna.pi[i], Cresceu, Retorno, ApRetorno);

    if (*Cresceu)
    {

        if (Ap->uniao.interna.ni < M * 2)
        {
            for (int j = Ap->uniao.interna.ni; j > i; j--)
            {
                Ap->uniao.interna.ri[j] = Ap->uniao.interna.ri[j - 1];
                Ap->uniao.interna.pi[j + 1] = Ap->uniao.interna.pi[j];
            }

            Ap->uniao.interna.ri[i] = Retorno->Chave;
            Ap->uniao.interna.pi[i + 1] = *ApRetorno;
            Ap->uniao.interna.ni++;
            *Cresceu = 0;
            return;
        }

        ApTemp = (TipoAp)malloc(sizeof(TipoPag));
        if (!ApTemp)
        {
            printf("Erro de alocação de memória!\n");
            exit(EXIT_FAILURE);
        }

        ApTemp->pt = Interna;
        int l = 0;

        // Transfere metade superior das chaves e apontadores para a nova página
        for (int j = M; j < M * 2; j++)
        {
            ApTemp->uniao.interna.ri[l] = Ap->uniao.interna.ri[j];
            ApTemp->uniao.interna.pi[l] = Ap->uniao.interna.pi[j];
            l++;

            // Remove o apontador original para evitar duplicação
            Ap->uniao.interna.pi[j] = NULL;
        }
        
        ApTemp->uniao.interna.pi[l] = Ap->uniao.interna.pi[M * 2];
        Ap->uniao.interna.pi[M * 2] = NULL;

        // Ajusta os tamanhos das páginas
        ApTemp->uniao.interna.ni = l;
        Ap->uniao.interna.ni = M;

        // Decide onde inserir a nova chave
        resultados->pesquisa.comparacoes++; // comparação do if
        resultados->pesquisa.comparacoes++; // comparação do else if
        
        if (Retorno->Chave < Ap->uniao.interna.ri[M - 1])
        {
            int x;
            for (x = Ap->uniao.interna.ni; x > 0 && Retorno->Chave < Ap->uniao.interna.ri[x - 1]; x--)
            {
                resultados->pesquisa.comparacoes += 1;
                Ap->uniao.interna.ri[x] = Ap->uniao.interna.ri[x - 1];
                Ap->uniao.interna.pi[x + 1] = Ap->uniao.interna.pi[x];
            }
            Ap->uniao.interna.ri[x] = Retorno->Chave;
            Ap->uniao.interna.pi[x + 1] = *ApRetorno;
        }
        
        else if (Retorno->Chave > Ap->uniao.interna.ri[M])
        {
            int x;

            Ap->uniao.interna.pi[M] = ApTemp->uniao.interna.pi[0];

            for (int i = 0; i < M-1; i++)
            {
                ApTemp->uniao.interna.ri[i] = ApTemp->uniao.interna.ri[i+1];
                ApTemp->uniao.interna.pi[i] = ApTemp->uniao.interna.pi[i+1];
            }
            
            ApTemp->uniao.interna.pi[M-1] = ApTemp->uniao.interna.pi[M];
            
            ApTemp->uniao.interna.ni--;

            for (x = ApTemp->uniao.interna.ni; x > 0 && Retorno->Chave < ApTemp->uniao.interna.ri[x - 1]; x--)
            {
                resultados->pesquisa.comparacoes += 1;
                ApTemp->uniao.interna.ri[x] = ApTemp->uniao.interna.ri[x - 1];
                ApTemp->uniao.interna.pi[x + 1] = ApTemp->uniao.interna.pi[x];
            }
            ApTemp->uniao.interna.ri[x] = Retorno->Chave;
            ApTemp->uniao.interna.pi[x + 1] = *ApRetorno;
            ApTemp->uniao.interna.ni++;
        }

        else{
            Ap->uniao.interna.ri[M] = Retorno->Chave;
            Ap->uniao.interna.pi[M] = ApTemp->uniao.interna.pi[0];
            ApTemp->uniao.interna.pi[0] = *ApRetorno;
        }

        // Promove a chave do meio
        Retorno->Chave = Ap->uniao.interna.ri[M];
        *ApRetorno = ApTemp;
        *Cresceu = 1;
    }
}

void InsereReg(Resultados *resultados, TipoRegistro registro, TipoAp *Ap)
{
    short Cresceu;
    TipoRegistro Retorno;
    TipoAp ApRetorno;

    if (*Ap == NULL)
    {
        TipoAp PagExterna = (TipoAp)malloc(sizeof(TipoPag));
        if (!PagExterna)
        {
            printf("Erro de alocação de memória!\n");
            exit(EXIT_FAILURE);
        }
        PagExterna->pt = Externa;
        PagExterna->uniao.externa.ne = 0;
        InserePaginaExterna(resultados, PagExterna, registro);

        *Ap = PagExterna;
        return;
    }

    InsereArv(resultados, registro, *Ap, &Cresceu, &Retorno, &ApRetorno);
    if (Cresceu)
    {
        TipoAp NovaRaiz = (TipoAp)malloc(sizeof(TipoPag));
        if (!NovaRaiz)
        {
            printf("Erro de alocação de memória!\n");
            exit(EXIT_FAILURE);
        }
        NovaRaiz->pt = Interna;
        NovaRaiz->uniao.interna.ni = 1;
        NovaRaiz->uniao.interna.ri[0] = Retorno.Chave;
        NovaRaiz->uniao.interna.pi[0] = *Ap;
        NovaRaiz->uniao.interna.pi[1] = ApRetorno;

        *Ap = NovaRaiz;
    }
}

void CriaArvoreBest(Resultados *resultados, FILE *arquivo, int quantidade, TipoAp *Arvore)
{
    TipoRegistro registro;
    int cont = 0;

    while ((fread(&registro, sizeof(registro), 1, arquivo) == 1) && cont < quantidade)
    {
        resultados->preProcessamento.transferencias += 1;

        if (!PesquisaBest(resultados, &registro, Arvore))
        {
            InsereReg(resultados, registro, Arvore);
            cont++;
        }
        else
            printf("\nREGISTRO EXISTE");
    }
}

void LiberaArvoreBest(TipoAp *arvore)
{
    // Verifica se a árvore está vazia
    if (*arvore == NULL)
    {
        return;
    }

    if ((*arvore)->pt == Interna)
    {
        // Para páginas internas, primeiro libera todos os filhos
        for (int i = 0; i <= (*arvore)->uniao.interna.ni; i++)
        {
            if ((*arvore)->uniao.interna.pi[i] != NULL)
            {
                LiberaArvoreBest(&((*arvore)->uniao.interna.pi[i]));
            }
        }
    }

    // Depois de liberar todos os filhos (ou se for página externa),
    // libera o nó atual
    free(*arvore);
    *arvore = NULL; // Evita ponteiros pendentes
}
