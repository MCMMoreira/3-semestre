#include "arvoreb.h"

// inicializar a árvore vazia
void Inicializar(TipoApontador *Arvore)
{
    *Arvore = NULL;
}

// pesquisar registro
bool Pesquisa(Resultados *resultados, TipoRegistro *registro, TipoApontador Ap, int chave)
{
    if (Ap == NULL)
    { // verificar se é vazia
        return false;
    }

    // percorre os elementos da pagina até encontrar algum maior que a chave
    int i = 1;
    resultados->pesquisa.comparacoes += 1;

    while (i < Ap->n && chave > Ap->r[i - 1].Chave)
    {
        i++;
        resultados->pesquisa.comparacoes += 1;
    }

    // verifica se a chave foi encontrada
    resultados->pesquisa.comparacoes += 1;
    if (i <= Ap->n && chave == Ap->r[i - 1].Chave)
    {
        *registro = Ap->r[i - 1]; 
        resultados->pesquisa.comparacoes += 1;
        return true;
    }

    // verifica se a busca segue pela esquerda ou direita
    resultados->pesquisa.comparacoes += 1;
    if (chave < Ap->r[i - 1].Chave)
    {
        return Pesquisa(resultados, registro, Ap->p[i - 1], chave);
    }
    else
    {
        return Pesquisa(resultados, registro, Ap->p[i], chave);
    }
}

// Insere registro na árvore
void InsereArvore(Resultados *resultados, TipoRegistro registro, TipoApontador Ap, bool *Cresceu, TipoRegistro *Retorno, TipoApontador *ApRetorno)
{
    int i = 1;
    TipoApontador ApTemp;

    if (Ap == NULL)
    {                       
        *Cresceu = true;     
        *Retorno = registro; 
        *ApRetorno = NULL; 
        return;
    }

    // Encontra a posição em que a chave deve ser inserida
    resultados->preProcessamento.comparacoes += 1;
    while (i < Ap->n && registro.Chave > Ap->r[i - 1].Chave)
    {
        i++;
        resultados->preProcessamento.comparacoes += 1;
    }

    // Verifica se a chave já está na árvore
    resultados->preProcessamento.comparacoes += 1;
    if (registro.Chave == Ap->r[i - 1].Chave)
    {
        *Cresceu = false;
        return;
    }

    // Verifica se a chave que será inserida é menor que a chave na posição atual
    resultados->preProcessamento.comparacoes += 1;
    if (registro.Chave < Ap->r[i - 1].Chave)
    {
        i--; 
    }

    // chamada recursiva da função
    InsereArvore(resultados, registro, Ap->p[i], Cresceu, Retorno, ApRetorno);

    if (!*Cresceu)
    {
        return;
    }

    // Insere o elemento caso o nó tenha espaço
    if (Ap->n < MM)
    {
        InserePagina(resultados, Ap, *Retorno, *ApRetorno);
        *Cresceu = false; 
        return;
    }

    // Se o nó estiver cheio, realiza a divisão
    ApTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    // Verifica onde será inserido o novo item durante a divisão
    if (i < M + 1)
    {
        InserePagina(resultados, ApTemp, Ap->r[MM - 1], Ap->p[MM]);
        Ap->n--;
        InserePagina(resultados, Ap, *Retorno, *ApRetorno);
    }
    else
    {
        InserePagina(resultados, ApTemp, *Retorno, *ApRetorno);
    }

    // Ajusta a metade dos elementos no novo nó
    for (int j = M + 2; j <= MM; j++)
    {
        InserePagina(resultados, ApTemp, Ap->r[j - 1], Ap->p[j]);
    }

    Ap->n = M;                   // O número de registros do nó recebe M
    ApTemp->p[0] = Ap->p[M + 1]; // Ajusta os ponteiros
    *Retorno = Ap->r[M];         // Retorno recebe o registro do meio
    *ApRetorno = ApTemp;         // Ajusta o apontador para a nova página
}

// insere um novo registro em uma árvore B
void InsereRegistro(Resultados *resultados, TipoRegistro registro, TipoApontador *Ap)
{
    bool Cresceu;
    TipoRegistro Retorno; 
    TipoPagina *ApRetorno, *ApTemp;

    // chama InsereArvore para inserir um novo registro, atualizando as variáveis Creceu, Retorno e ApRetorno
    InsereArvore(resultados, registro, *Ap, &Cresceu, &Retorno, &ApRetorno);

    // Se if for verdadeiro, cria uma nova raiz
    if (Cresceu)
    {
        ApTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = Retorno; 
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
    }
}

void InserePagina(Resultados *resultados, TipoApontador Ap, TipoRegistro Registro, TipoApontador ApDir)
{
    int k = Ap->n; // k inicia no último registro da página

    // Abre espaço na página para o novo item
    for (; k > 0 && Registro.Chave < Ap->r[k - 1].Chave; k--)
    {

        resultados->preProcessamento.comparacoes += 1;

        Ap->r[k] = Ap->r[k - 1]; // desloca o maior registro para a direita
        Ap->p[k + 1] = Ap->p[k]; // ajusta o ponteiro
    }

    Ap->r[k] = Registro;  // insere o novo registro
    Ap->p[k + 1] = ApDir; // ajusta o ponteiro para a pagina a direita do novo registro
    Ap->n++;              // incrementa o numero de registros da página
}

// cria a árvore b
void CriaArvore(Resultados *resultados, FILE *arquivo, int quantidade, TipoApontador *Arvore)
{
    TipoRegistro registro;
    Inicializar(Arvore);

    // faz a leitura dos itens do arquivo e os insere na árvore
    for (int cont = 0; cont < quantidade && fread(&registro, sizeof(TipoRegistro), 1, arquivo) == 1; cont++)
    {
        InsereRegistro(resultados, registro, Arvore);

        resultados->preProcessamento.transferencias += 1;
    }   
}

// liberar a memória alocada
void liberarArvoreB(TipoApontador Arvore)
{
    if (Arvore == NULL)
    {
        return;
    }

    for (int i = 0; i <= Arvore->n; i++)
    {
        liberarArvoreB(Arvore->p[i]);
    }

    free(Arvore);
}
