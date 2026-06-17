#include <stdio.h>
#include <stdlib.h>

#include "asi.h"

// Acesso Sequencial Indexado

int geraIndice(Resultados *resultados, FILE *arq, TipoRegistro *tabela)
{
    TipoRegistro item;
    int i = 0, desloc;
     
    desloc = (ITENSPAGINA_ASI - 1) * sizeof(item);

    while (fread(&item, sizeof(item), 1, arq) == 1) // le o primeiro item da pagina
    {  
        resultados->preProcessamento.transferencias += 1;

        tabela[i] = item;// coloca o item na posicao correspondente do vetor (indice)

        fseek(arq, desloc, SEEK_CUR); // desloca para a proxima pagina

        i++;// retorna a quantidade de itens no vetor
    }
    return i;
}

bool pesquisa(Resultados *resultados, TipoRegistro *tab, int tam, TipoRegistro *item, FILE *arq, int situacao)
{
    TipoRegistro pagina[ITENSPAGINA_ASI];
    int i, quantitens;
    long desloc;

    i = 0;

    if (situacao == 1) // se o arquivo estiver em ordem crescente
    {
        resultados->pesquisa.comparacoes += 1;
        while (i < tam && tab[i].Chave <= item->Chave)
        {
            i++; 
            resultados->pesquisa.comparacoes += 1;
        }
    }

    else // se o arquivo estiver em ordem decrescente
    {
        resultados->pesquisa.comparacoes += 1;
        while (i < tam && tab[i].Chave >= item->Chave)
        {
            i++; 
            resultados->pesquisa.comparacoes += 1;
        }
    }

    if (i == 0) // Chave nao encontrada por ser menor ou maior que qualquer item no arquivo
        return false;

    if (i < tam) // item pode estar em uma das paginas do meio
    {
        quantitens = ITENSPAGINA_ASI;
    }

    else // item pode estar na ultima pagina
    {
        fseek(arq, 0, SEEK_END);
        quantitens = (ftell(arq) / sizeof(TipoRegistro) % ITENSPAGINA_ASI); // verifica a quantidade de paginas no arquivo

        // conferir caso a ultima pagina esteja completa

        if (quantitens == 0)
            quantitens = ITENSPAGINA_ASI;
    }

    desloc = (i - 1) * ITENSPAGINA_ASI * sizeof(TipoRegistro);
    fseek(arq, desloc, SEEK_SET);
    fread(&pagina, sizeof(TipoRegistro), quantitens, arq); // le a pagina em que o item possivelmente esta

    resultados->pesquisa.transferencias += 1;

    return buscaItem(resultados,pagina, item, situacao); // realiza a busca binaria na pagina
}

bool buscaItem(Resultados *resultados, TipoRegistro pagina[ITENSPAGINA_ASI], TipoRegistro *item, int situacao)
{
    int meio, esq = 0;
    int dir = ITENSPAGINA_ASI - 1;

    if (situacao == 1)
        while (esq <= dir)
        {
            meio = (esq + dir) / 2;
            
            resultados->pesquisa.comparacoes += 1;

            if (pagina[meio].Chave == item->Chave) // retorna true caso encontre o item
            {
                *item = pagina[meio];
                return true;
            }
   
            resultados->pesquisa.comparacoes += 1;

            if (pagina[meio].Chave < item->Chave)
                esq = meio + 1;

            else
            { 
                dir = meio - 1;

                resultados->pesquisa.comparacoes += 1;
            }
        }

    if (esq > dir)
        return false;

    while (esq <= dir)
    {
        meio = (esq + dir) / 2;
        
        resultados->pesquisa.comparacoes += 1;

        if (pagina[meio].Chave == item->Chave) // retorna true caso encontre o item
        {
            *item = pagina[meio];
            return true;
        } 
        resultados->pesquisa.comparacoes += 1;

        if (pagina[meio].Chave > item->Chave)
            esq = meio + 1;

        else
        {   
            dir = meio - 1;
            
            resultados->pesquisa.comparacoes += 1;
        }
    }

    return false; // item nao encontrado
}
