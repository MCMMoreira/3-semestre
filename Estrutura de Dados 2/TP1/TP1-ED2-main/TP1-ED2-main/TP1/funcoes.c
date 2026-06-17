#include <stdio.h>
#include <stdlib.h>

#include "funcoes.h"

// Valida os inputs passados pelo usuário
bool inputsValidados(int metodo, int quantidade, int situacao)
{
    bool inputsValidos = true;

    if (metodo < 1 || metodo > 4)
    {
        printf("\nMétodo inválido! Métodos válidos:");
        printf("\t1 - Acesso sequencial indexado;\n");
        printf("\t2 - Árvore binária de pesquisa;\n");
        printf("\t3 - Árvore B;\n");
        printf("\t4 - Árvore B*.\n");
        inputsValidos = false;
    }

    if (quantidade != 100 && quantidade != 200 && quantidade != 2000 &&
        quantidade != 20000 && quantidade != 200000 && quantidade != 2000000)
    {
        printf("\nQuantidade de itens inválida. Valores válidos:");
        printf("\t100, 200, 2000, 20000, 200000, 2000000.\n");
        inputsValidos = false;
    }

    if (situacao < 1 || situacao > 3)
    {
        printf("\nSituação inválida! Situações válidas:\n");
        printf("\t1 - Crescente;\n");
        printf("\t2 - Decrescente;\n");
        printf("\t3 - Aleatória.\n");
        inputsValidos = false;
    }

    if (metodo == 1 && situacao > 2)
    {
        printf("\nO método de acesso sequencial indexado não suporta dados organizados aleatoriamente.\n");
        inputsValidos = false;
    }

    return inputsValidos;
}

// Gera 20 numeros aleatorios para realizar a pesquisa automatizada
bool gerarNumerosAleatorios(char *nomeArquivo, int quantidade, Resultados *resultados)
{

    int posicao[20];
    TipoRegistro reg;

    FILE *arq = fopen(nomeArquivo, "rb");

    if (arq == NULL)
        return false;

    int n = 0;

    // Gera as 20 chaves aleatorias para pesquisar
    for (int i = 0; i < 20; i++)
    {

        int posicaoRand = rand() % quantidade;

        if (verificaChaveRepetida(posicaoRand, posicao, n))
        {
            while (verificaChaveRepetida(posicaoRand, posicao, n))
            {
                posicaoRand = rand() % quantidade;
            }
        }

        posicao[i] = posicaoRand;
        n++;

        // Lendo a chave e salvando no vetor de resultados
        fseek(arq, (posicao[i]) * sizeof(TipoRegistro), SEEK_SET);
        fread(&reg, sizeof(TipoRegistro), 1, arq);

        resultados[i].pesquisar.Chave = reg.Chave;
        resultados[i].pesquisar.Dado1 = reg.Dado1;
        strcpy(resultados[i].pesquisar.Dado2, reg.Dado2);

        resultados[i].pesquisarEstrela.chave = reg.Chave;
        resultados[i].pesquisarEstrela.dado1 = reg.Dado1;
        strcpy(resultados[i].pesquisarEstrela.dado2, reg.Dado2);
    }

    fclose(arq);
    return true;
}

// Verifica se ha uma chave repetida dentro de um vetor
bool verificaChaveRepetida(int posicao, int *posicoes, int tam)
{
    for (int i = 0; i < tam; i++)
        if (posicao == posicoes[i])
            return true;

    return false;
}

// Imprime as chaves presentes no arquivo
void imprimirChavesArquivo(char *nomeArquivo, int quantidade)
{
    FILE *arq;
    if ((arq = fopen(nomeArquivo, "rb")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        return;
    }

    TipoRegistro *registros = (TipoRegistro *)malloc(quantidade * sizeof(TipoRegistro));
    fread(registros, sizeof(TipoRegistro), quantidade, arq);

    printf("\n");
    printf(GREEN("\nCHAVES PRESENTES NO ARQUIVO:\n"));
    printf("\n");

    for (int i = 0; i < quantidade; i++)
    {
        printf("Chave: %d\n", registros[i].Chave);
    }

    free(registros);
    fclose(arq);
}

// Imprime todos os resultados obtidos a partir da pesquisa realizada
void calcularTempo(Resultados *resultado)
{
    // Calcula os tempos de processamento
    double tempoPreProcessamento = ((double)(resultado->tempoPreProcessamento[1] - resultado->tempoPreProcessamento[0])) / CLOCKS_PER_SEC;

    double tempoPesquisa = ((double)(resultado->tempoPesquisa[1] - resultado->tempoPesquisa[0])) / CLOCKS_PER_SEC;

    double tempoTotal = tempoPreProcessamento + tempoPesquisa;

    // Armazena os tempos calculados no resultado
    resultado->tempoPreProcessamentoCalculado = tempoPreProcessamento;
    resultado->tempoPesquisaCalculado = tempoPesquisa;
    resultado->tempoTotalCalculado = tempoTotal;
}

// Calcula a média dos tempos e estatísticas de execução
void imprimirResultados(Resultados *resultados, int NUM_EXECUCOES)
{
    // Inicializa os somatórios
    double somaTempoPreProcessamento = 0.0;
    double somaTempoPesquisa = 0.0;

    long somaTransferenciasPreProcessamento = 0;
    long somaTransferenciasPesquisa = 0;

    long somaComparacoesPreProcessamento = 0;
    long somaComparacoesPesquisa = 0;

    for (int i = 0; i < NUM_EXECUCOES; i++)
    {
        somaTempoPreProcessamento += resultados[i].tempoPreProcessamentoCalculado;
        somaTempoPesquisa += resultados[i].tempoPesquisaCalculado;
        somaTransferenciasPreProcessamento += resultados[i].preProcessamento.transferencias;
        somaTransferenciasPesquisa += resultados[i].pesquisa.transferencias;
        somaComparacoesPreProcessamento += resultados[i].preProcessamento.comparacoes;
        somaComparacoesPesquisa += resultados[i].pesquisa.comparacoes;
    }

    double tempoMedioPreProcessamento = somaTempoPreProcessamento;

    double tempoMedioPesquisa = somaTempoPesquisa / NUM_EXECUCOES;
    double tempoMedioTotal = tempoMedioPreProcessamento + tempoMedioPesquisa;

    long transferenciasMediaPreProcessamento = somaTransferenciasPreProcessamento;

    long transferenciasMediaPesquisa = somaTransferenciasPesquisa / NUM_EXECUCOES;
    long transferenciasMediaTotal = transferenciasMediaPreProcessamento + transferenciasMediaPesquisa;

    long comparacoesMediaPreProcessamento = somaComparacoesPreProcessamento;

    long comparacoesMediaPesquisa = somaComparacoesPesquisa / NUM_EXECUCOES;
    long comparacoesMediaTotal = comparacoesMediaPreProcessamento + comparacoesMediaPesquisa;

    printf("\n\n");

    printf(GREEN("\nESTATÍSTICAS\n"));

    printf(YELLOW("\nPRÉ-PROCESSAMENTO\n"));

    printf("Tempo de execução:   %10.6lf s\n", tempoMedioPreProcessamento);
    printf("Comparações:         %10ld\n", comparacoesMediaPreProcessamento);
    printf("Transferências:      %10ld\n", transferenciasMediaPreProcessamento);

    printf(YELLOW("\nPESQUISA\n"));
    printf("Tempo de execução:   %10.6lf s\n", tempoMedioPesquisa);
    printf("Comparações:         %10ld\n", comparacoesMediaPesquisa);
    printf("Transferências:      %10ld\n", transferenciasMediaPesquisa);

    printf(YELLOW("\nTOTAL\n"));
    printf("Tempo de execução:   %10.6lf s\n", tempoMedioTotal);
    printf("Comparações:         %10ld\n", comparacoesMediaTotal);
    printf("Transferências:      %10ld\n", transferenciasMediaTotal);

    printf("\n\n");
}
