#include <stdbool.h>
#include "arvorebin.h"

#define NULO -1

FILE *constroiArvore(Resultados *resultados, FILE *arquivo)
{
    if (arquivo == NULL) {
        printf("Erro: ao abrir o arquivo \n");
        return NULL;
    }

    FILE *arvore;
    TipoRegistro item;
    Nodo novo, aux;
    int n = 1;

    arvore = fopen("arvorebin.bin", "wb+");
 
    fread(&item, sizeof(item), 1, arquivo); // lê o primeiro item para iniciar a árvore 

    resultados->preProcessamento.transferencias += 1;

    // item auxiliar que sera utilizado como raiz da arvore
    memset(&aux, 0, sizeof(aux));

    aux.item = item;
    aux.dir = NULO;
    aux.esq = NULO;
    aux.pos = 0;

    fwrite(&aux, sizeof(Nodo), 1, arvore); // inicializa a arvore com o primeiro valor 

    // inicializa o novo nodo
    memset(&novo, 0, sizeof(novo));

    novo.dir = NULO;
    novo.esq = NULO;
    novo.pos = 0;
    novo.item.Chave = 0; 
    novo.item.Dado1 = 0; 
    
    memset(novo.item.Dado2, 0, TAM); 

    while (fread(&item, sizeof(item), 1, arquivo) == 1)
    {
        
        resultados->preProcessamento.transferencias += 1;

        // insere o item na arvore
        fseek(arvore, 0, SEEK_SET);

        novo.pos = n;
        novo.item = item;

        if (insereNodo(resultados, novo, arvore, n))
            n++; // incrementa o numero de nodos
    }

    return arvore;
}

bool insereNodo(Resultados *resultados, Nodo novo, FILE *arvore, int numNodos)
{
    int desloc;
    Nodo aux;

    while (fread(&aux, sizeof(aux), 1, arvore) == 1)
    {
         
        resultados->preProcessamento.transferencias += 1;
        resultados->preProcessamento.comparacoes += 1;

        if (novo.item.Chave > aux.item.Chave)
        {     
            resultados->preProcessamento.comparacoes += 1;

            if (aux.dir == NULO)
            {
                aux.dir = numNodos;

                fseek(arvore, -(sizeof(aux)), SEEK_CUR); // faz o cursor retornar para aux
                fwrite(&aux, sizeof(aux), 1, arvore);    // reescreve o item com o novo "ponteiro" para a direita
                

                fseek(arvore, 0, SEEK_END);
                fwrite(&novo, sizeof(novo), 1, arvore); // insere o novo item no fim do arquivo


                return true;
            }

            desloc = aux.dir * sizeof(aux);
        }
        else if (novo.item.Chave < aux.item.Chave)
        {
            resultados->preProcessamento.comparacoes += 1;

            resultados->preProcessamento.comparacoes += 1;

            if (aux.esq == NULO)
            {
                aux.esq = numNodos;

                fseek(arvore, -(sizeof(aux)), SEEK_CUR); // faz o cursor retornar para aux

                fwrite(&aux, sizeof(aux), 1, arvore); // reescreve o item com o novo "ponteiro" para a esquerda

                fseek(arvore, 0, SEEK_END);
                fwrite(&novo, sizeof(novo), 1, arvore); // insere o novo item no fim do arquivo

                return true;
            }

            desloc = aux.esq * sizeof(aux);
        }

        fseek(arvore, desloc, SEEK_SET); // muda o cursor para a proxima posicao a ser lida de acordo com o "ponteiro"
    }

    return false;
}

bool pesquisaItem(Resultados *resultados, TipoRegistro *item, Nodo pagina[ITENSPAGINA], Nodo aux, int *posRet)
{
    int desloc, posvetor;

    resultados->pesquisa.comparacoes += 1;

    if (item->Chave > aux.item.Chave)
    {
        
        resultados->pesquisa.comparacoes += 1;

        if (aux.dir == NULO) // valor nao esta na arvore
        {
            return false;
        }
        else
        {   
            resultados->pesquisa.comparacoes += 1;

            desloc = aux.dir - aux.pos; // deslocamento relativo a posicao atual de aux

            posvetor = aux.pos % ITENSPAGINA; // a posicao de aux dentro do vetor pagina de acordo com a posicao no arquivo
            
            resultados->pesquisa.comparacoes += 1;

            if (posvetor + desloc < ITENSPAGINA) // confere se o ponteiro aponta para um valor que esta na pagina
            {
                aux = pagina[posvetor + desloc];
                return pesquisaItem(resultados, item, pagina, aux, posRet); // salta para o filho dentro da pagina e chama a funcao recursivamente
            }

            *posRet = aux.dir; // retorna por parametro a posicao do proximo filho

            return false;
        }
    }
    else if (item->Chave < aux.item.Chave)
    { 
        resultados->pesquisa.comparacoes += 1;

        resultados->pesquisa.comparacoes += 1;

        if (aux.esq == NULO) // valor nao esta na arvore
        {
            return false;
        }
        else
        {
            
            resultados->pesquisa.comparacoes += 1;

            desloc = aux.esq - aux.pos; // deslocamento relativo a posicao atual de aux

            posvetor = aux.pos % ITENSPAGINA; // a posicao de aux dentro do vetor pagina de acordo com a posicao no arquivo
            
            resultados->pesquisa.comparacoes += 1;

            if (posvetor + desloc < ITENSPAGINA) // confere se o ponteiro aponta para um valor que esta na pagina
            {
                aux = pagina[posvetor + desloc];
                return pesquisaItem(resultados, item, pagina, aux, posRet); // salta para o filho dentro da pagina e chama a funcao recursivamente
            }

            *posRet = aux.esq; // retorna por parametro a posicao do proximo filho

            return false;
        }
    }

    else if (item->Chave == aux.item.Chave)
    {
        resultados->pesquisa.comparacoes += 1;

        *item = aux.item;
        return true;
    }

    return false;
}
