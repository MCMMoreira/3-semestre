#include "include/funcoes.h"

void transfereArquivo (FILE *arq, int situacao, int quantidade)
{
    Aluno aluno;
    FILE *arqTxt = fopen ("provao.TXT", "w+");

    for (int i = 0; i < quantidade; i++)
    { 
        fread(&aluno, sizeof(Aluno), 1, arq);
    
        fprintf (arqTxt, "%-8ld %lf %-2s %-50s %-30s\n", aluno.inscricao, aluno.nota, aluno.estado, aluno.cidade, aluno.curso); //escreve no arquivo texto as informacoes do arquivo binario
    }

    fclose(arqTxt);
}

void calcularTempo(Resultados *resultado)
{
    double tempoOrdenacao = ((double)(resultado->tempoOrdenacao[1] - resultado->tempoOrdenacao[0])) / CLOCKS_PER_SEC;

    // Armazena os tempos calculados no resultado
    resultado->tempoOrdenacaoCalculado = tempoOrdenacao;
}

void calcularTempoSelecao(Resultados *resultado)
{

    double tempoSelecao = ((double) (resultado->tempoSelecao[1] - resultado->tempoSelecao[0])) / CLOCKS_PER_SEC; 

    // Armazena os tempos calculados no resultado
    resultado->tempoSelecaoCalculado = tempoSelecao; 
}

void imprimirResultados(Resultados *resultados)
{
    calcularTempo(resultados);
    
    printf("\n\n");

    printf(GREEN("\nESTATÍSTICAS\n"));

    printf(YELLOW("\nORDENAÇÃO\n"));
    printf("Tempo de execução:           %10.6lf s\n", resultados->tempoOrdenacaoCalculado);
    printf("Comparações:                 %10ld\n", resultados->ordenacao.comparacoes);
    printf("Transferências Leitura:      %10ld\n", resultados->ordenacao.leitura);
    printf("Transferências Escrita:      %10ld\n", resultados->ordenacao.escrita);

    printf("\n\n");
}

void imprimirResultadosSelecao(Resultados *resultados)
{
    calcularTempoSelecao(resultados);
    
    printf("\n\n");

    printf(GREEN("\nESTATÍSTICAS\n"));

    printf(YELLOW("\nSeleção por Substituição\n"));
    printf("Tempo de execução:           %10.6lf s\n", resultados->tempoSelecaoCalculado);
    printf("Comparações:                 %10ld\n", resultados->selecao.comparacoes);
    printf("Transferências Leitura:      %10ld\n", resultados->selecao.leitura);
    printf("Transferências Escrita:      %10ld\n", resultados->selecao.escrita);

    printf("\n\n");
}

FILE* abrirArquivo(int situacao, char* modoAbertura, int tipo)
{
    FILE* arq;
    
    if (tipo == 0) //arquivos texto
    {
        switch (situacao){
            case 1: //ordem crescente
                arq = fopen ("provao-1.TXT", modoAbertura);
                break;
            
            case 2: //ordem decrescente
                arq = fopen ("provao-2.TXT", modoAbertura);
                break;
            
            case 3: //ordem aleatoria
                arq = fopen ("provao-3.TXT", modoAbertura);
                break;
            
            default:
                printf ("\nSituação Inválida!");
                return NULL;
        }
    }
    
    else if (tipo == 1) //arquivos binarios
    {
        switch (situacao){
            case 1: //ordem crescente
                arq = fopen ("provaoBin-1.bin", modoAbertura);
                break;
            
            case 2: //ordem decrescente
                arq = fopen ("provaoBin-2.bin", modoAbertura);
                break;
            
            case 3: //ordem aleatoria
                arq = fopen ("provaoBin-3.bin", modoAbertura);
                break;
            
            default:
                printf ("\nSituação Inválida!");
                return NULL;
        }
    }

    return arq;
}

