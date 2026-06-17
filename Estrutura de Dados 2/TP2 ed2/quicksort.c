#include "include/quicksort.h"

void QuickSortExterno (FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir, Resultados* resultados)
{
    int i, j;
    TipoArea Area;

    if (Dir - Esq < 1)
        return;

    FAVazia(Area); //esvazia todos os campos da area (pivo)

    Particao (ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j, resultados);

    //garante que as informacoes da particao anterior sejam escritas no arquivo
    fflush(*ArqLi);
    fflush(*ArqEi);
    fflush(*ArqLEs);

    //comeca pelo subarquivo de menor tamanho
    if (i - Esq < Dir -j) 
    {
        QuickSortExterno (ArqLi, ArqEi, ArqLEs, Esq, i, resultados);
        QuickSortExterno (ArqLi, ArqEi, ArqLEs, j, Dir, resultados);
    }

    else
    {
        QuickSortExterno (ArqLi, ArqEi, ArqLEs, j, Dir, resultados);
        QuickSortExterno (ArqLi, ArqEi, ArqLEs, Esq, i, resultados);
    }
}

void LeSup(FILE **ArqLEs, Aluno* UltLido, int *Ls, short *OndeLer, Resultados* resultados)
{
    //posiciona o ponteiro para ler o proximo item da parte mais a direita
    fseek (*ArqLEs, (*Ls - 1) * sizeof (Aluno), SEEK_SET);
    fread(UltLido, sizeof(Aluno), 1, *ArqLEs);
    resultados->ordenacao.leitura++;

    (*Ls)--;

    *OndeLer = false;
}

void LeInf(FILE **ArqLi, Aluno* UltLido, int *Li, short *OndeLer, Resultados* resultados)
{
    //posiciona o ponteiro para ler o proximo item da parte mais a esquerda
    fread(UltLido, sizeof(Aluno), 1, *ArqLi);
    resultados->ordenacao.leitura++;

    (*Li)++;

    *OndeLer = true;
}

void InserirArea (TipoArea Area, Aluno* UltLido, int *NRArea, Resultados* resultados)
{
    InsereItem (UltLido, Area, resultados);

    *NRArea = ObterNumCelOcupadas(Area); //atualiza o numero de itens no pivo
}

void EscreveMax (FILE **ArqLEs, Aluno R, int *Es, Resultados* resultados)
{
    fseek(*ArqLEs, (*Es - 1) * sizeof(Aluno), SEEK_SET);

    //escreve o item no 'subarquivo' com os maiores itens
    fwrite(&R, sizeof(Aluno), 1, *ArqLEs);
    resultados->ordenacao.escrita++;
 
    (*Es)--;
}

void EscreveMin (FILE **ArqEi, Aluno R, int *Ei, Resultados* resultados)
{
    fseek(*ArqEi, (*Ei - 1) * sizeof(Aluno), SEEK_SET);

    //escreve o item no 'subarquivo' com os menores itens
    fwrite(&R, sizeof(Aluno), 1, *ArqEi);
    resultados->ordenacao.escrita++;

    (*Ei)++;
}

void RetiraMax(TipoArea Area, Aluno* R, int *NRArea)
{
    RetiraUltimo (R, Area);

    *NRArea = ObterNumCelOcupadas(Area); //atualiza o numero de itens no pivo
}

void RetiraMin(TipoArea Area, Aluno* R, int *NRArea)
{
    RetiraPrimeiro (R, Area);

    *NRArea = ObterNumCelOcupadas(Area); //atualiza o numero de itens no pivo
}

void Particao (FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j, Resultados* resultados)
{
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0, Linf = INT_MIN, Lsup = INT_MAX;

    short OndeLer = true;

    Aluno R;

    //atualiza os ponteiros para o inicio do arquivo/ou subarquivo
    fseek (*ArqLi, (Li-1) * sizeof(Aluno), SEEK_SET);
    fseek (*ArqEi, (Ei-1) * sizeof(Aluno), SEEK_SET);

    *i = Esq - 1;
    *j = Dir + 1;

    while (Ls >= Li)
    {
        Aluno UltLido = {0}; //inicializa os campos com valores neutros
   
        if (NRArea < MEM_EXT_QUICK - 1) //se o item a ser inserido no pivo nao for ocupar a ultima posicao disponivel
        {
            if (OndeLer)
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, resultados);
            
            else
                LeInf(ArqLi, &UltLido, &Li, &OndeLer, resultados);
            
            InserirArea (Area, &UltLido, &NRArea, resultados);

            continue;
        }

        //confere se a ordem de revezamento tradicional deve ser quebrada para nao sobrescrever nenhum item antes de ser lido
        if (Ls == Es)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, resultados);
        
        else if (Li == Ei)
            LeInf(ArqLi, &UltLido, &Li, &OndeLer, resultados);

        //segue a ordem tradicional de revezamento
        else if (OndeLer)
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer, resultados);
        
        else
            LeInf(ArqLi, &UltLido, &Li, &OndeLer, resultados);


        resultados->ordenacao.comparacoes++;
        if (UltLido.nota * 100 > Lsup)
        {
            *j = Es; //aumenta o delimitador do subarquivo superior, caso o ultimo item lido esteja fora dos limites do pivo por ser maior

            EscreveMax(ArqLEs, UltLido, &Es, resultados);
            continue;
        }

        resultados->ordenacao.comparacoes++;
        if (UltLido.nota * 100 < Linf)
        {
            *i = Ei; //aumenta o delimitador do subarquivo inferior, caso o ultimo item lido esteja fora dos limites do pivo por ser menor 

            EscreveMin(ArqEi, UltLido, &Ei, resultados);
            continue;
        }

        InserirArea(Area, &UltLido, &NRArea, resultados);

        //retira um item do pivo de acordo com a subpagina com menos itens
        if (Ei-Esq < Dir-Es)
        {
            RetiraMin(Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei, resultados);

            Linf = R.nota * 100;
        }

        else
        {
            RetiraMax(Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es, resultados);

            Lsup = R.nota * 100;
        }        
    }

    while (Ei <= Es) //escreve os itens restantes do pivo nas posicoes corretas no arquivo 
    {
        RetiraMin(Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei, resultados);
    }
}

void FAVazia(TipoArea Area) 
{
    for (int i = 0; i < MEM_EXT_QUICK; i++) 
    {
        Area[i].inscricao = -1;  
        Area[i].nota = -1.0;  
        Area[i].estado[0] = '\0';  
        Area[i].cidade[0] = '\0';
        Area[i].curso[0] = '\0';
    }
}

void InsereItem (Aluno* UltLido, TipoArea Area, Resultados* resultados)
{
    int i = ObterNumCelOcupadas (Area);

    while (UltLido->nota * 100 < Area[i-1].nota * 100 && i != 0) //encontra a posicao do novo item ordenadamente no pivo
    {
        resultados->ordenacao.comparacoes++;

        Area[i] = Area[i-1];

        i--;
    }

    //insere na posicao correta
    Area[i] = *UltLido;
}

void RetiraUltimo (Aluno* UltLido, TipoArea Area)
{
    int i = ObterNumCelOcupadas (Area);

    *UltLido = Area[i-1];

    Area[i-1].nota = -1; //marca a ultima posicao como vazia
}

void RetiraPrimeiro (Aluno* UltLido, TipoArea Area)
{
    int i = ObterNumCelOcupadas (Area);

    *UltLido = Area[0];

    for (int j = 0; j < i-1; j++) // desloca os itens da frente para encobrir o espaco vazio do primeiro item
    {
        Area[j] = Area[j+1];
    }

    Area[i-1].nota = -1;
}

int ObterNumCelOcupadas (TipoArea Area)
{
    int i = 0;

    while (i < MEM_EXT_QUICK)
    {
        if (Area[i].nota != -1)
            i++;
        
        else
            break;
    }

    return i;
}
