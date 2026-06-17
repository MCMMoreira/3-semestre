#include "include/funcoes.h"
#include "include/quicksort.h"
#include "include/selecao.h"
#include "include/inter2f.h"

void quickSort(int quantidade, int situacao, Resultados *resultados);
void SeleçãoFmaisUm(int quantidade, int situacao, Resultados *resultados);
void intercalacao2Fitas(int quantidade, int situacao, Resultados *resultados);

int main(int argc, char *argv[])
{
    Resultados resultados;
    Aluno aux;
    FILE* arq;

    if (argc < 4 || argc > 5) // confere se a quantidade de argumentos esta correta
    {
        printf(RED("\nQuantidade de Argumentos Inválida\n"));
        printf("\t./nome_executavel <metodo> <quantidade_de_itens> <situacao> [-p]\n");
        return 0;
    }

    int metodo = atoi(argv[1]);
    int quantidade = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    bool p = false;

    memset(&resultados, 0, sizeof(resultados)); // inicializa todas as estatisticas com 0

    if (argc == 5)
    {
        if (strcmp(argv[4], "-P") == 0) //imprime o arquivo antes da ordenacao caso tenha sido escrito o parametro -P
        {
            arq = abrirArquivo(situacao, "r", 0);

            if (!arq)
            {
                printf ("\nErro ao abrir o arquivo!");
                return 1;
            }

            for (int i = 0; i <quantidade; i ++)
            {
                fscanf(arq, "%8ld %lf %2s %50[^\n] %30[^\n]", &aux.inscricao, &aux.nota, aux.estado, aux.cidade, aux.curso);
                printf ("%8.ld %lf %2s %s %s", aux.inscricao, aux.nota, aux.estado, aux.cidade, aux.curso);

                printf("\n");
            }

            fclose(arq);

            p = true;
        }
    }

    switch (metodo)
    {
        case 1:
            intercalacao2Fitas(quantidade, situacao, &resultados);
            break;
        
        case 2:
            SeleçãoFmaisUm(quantidade, situacao, &resultados);
            break;

        case 3:
            quickSort(quantidade, situacao, &resultados);
            break;
        
        default:
            printf("\nMétodo inválido!\n");
            return 1;
    }
    
    if (p == true)
    {
        printf ("\n\n\n");

        arq = fopen ("provao.TXT", "r");

        if (!arq)
        {
            printf ("\nErro ao abrir o arquivo!");

            return 1;
        }

        for (int i = 0; i <quantidade; i++) //imprime o arquivo depois da ordenacao caso tenha sido escrito o parametro -P
        {
            fscanf(arq, "%8ld %lf %2s %50[^\n] %30[^\n]", &aux.inscricao, &aux.nota, aux.estado, aux.cidade, aux.curso);
            printf ("%8.ld %lf %2s %s %s", aux.inscricao, aux.nota,aux.estado, aux.cidade, aux.curso);

            printf("\n");
        }

        fclose(arq);
    }

    if(metodo == 1 || metodo == 2){

        imprimirResultadosSelecao(&resultados);
    }
    
    imprimirResultados(&resultados);

    return 0;
}

void quickSort(int quantidade, int situacao, Resultados *resultados)
{
    FILE *ArqLEs, *ArqLi, *ArqEi;

    ArqLi = abrirArquivo (situacao, "r+b", 1);
    ArqEi = abrirArquivo (situacao, "r+b", 1);
    ArqLEs = abrirArquivo (situacao, "r+b", 1);

    if (!ArqLi || !ArqEi || !ArqLEs)
    {
        printf ("\nErro ao abrir o arquivo\n");
        return;
    }

    resultados->tempoOrdenacao[0] = clock();

    QuickSortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, quantidade, resultados);

    resultados->tempoOrdenacao[1] = clock();

    fflush(ArqLi);
    fclose(ArqEi);
    fclose(ArqLEs);
    fseek(ArqLi, 0, SEEK_SET);

    transfereArquivo(ArqLi, situacao, quantidade);

    fclose(ArqLi);
}

void SeleçãoFmaisUm(int quantidade, int situacao, Resultados *resultados)
{
    FILE *arquivo, *fitaFinal, *arquivoFinal;
    char linha[256];
    int contador = 0;

    arquivo = abrirArquivo(situacao, "r+", 0);
    if (!arquivo)
    {
        printf("\nErro ao abrir o arquivo\n");
        return;
    }

    resultados->tempoSelecao[0] = clock();
    selecaoPorSubstituicao(arquivo, quantidade, resultados);
    resultados->tempoSelecao[1] = clock();
    
    resultados->tempoOrdenacao[0] = clock();
    ordenarFitas(resultados);
    resultados->tempoOrdenacao[1] = clock();

    fclose(arquivo);

    fitaFinal = fopen("Fitas/fita_20.txt", "r");
    arquivoFinal = fopen("provao.TXT", "w");

    if (!fitaFinal || !arquivoFinal)
    {
        printf("\nErro ao abrir a fita_20.txt\n");
        return;
    }

    // Copiar registros para provao
    while (contador < quantidade && fgets(linha, sizeof(linha), fitaFinal))
    {
        fputs(linha, arquivoFinal);
        contador++;
    }

    fclose(fitaFinal);
    fclose(arquivoFinal);
    
}

void intercalacao2Fitas(int quantidade, int situacao, Resultados *resultados)
{
    FILE *arquivo, *fitaFinal, *arquivoFinal;
    arquivo = abrirArquivo(situacao, "r+", 0);

    char linha[256];
    int contador = 0;

    if (!arquivo)
    {
        printf("\nErro ao abrir o arquivo\n");
        return;
    }

    resultados->tempoSelecao[0] = clock();
    selecaoPorSubstituicao(arquivo, quantidade, resultados);
    resultados->tempoSelecao[1] = clock();
    
    resultados->tempoOrdenacao[0] = clock();
    int iteracao = intercalarFitasOrdenadas2F(resultados); // Recebe o valor de retorno da função
    resultados->tempoOrdenacao[1] = clock();

    // Verifica se ocorreu um erro durante a intercalação
    if (iteracao == -1) {
        printf("\nErro durante o processo de intercalação. Abortando operação.\n");
        fclose(arquivo); // Fecha o arquivo aberto anteriormente
        return; // Interrompe a execução da função
    }

    // Determina qual fita deve ser aberta com base no valor de iteracao
    int fitaNumero = (iteracao % 2 == 0) ? 11 : 1; // Se iteracao for par, fita 11; se ímpar, fita 1
    char nomeFitaFinal[30];
    snprintf(nomeFitaFinal, sizeof(nomeFitaFinal), "Fitas/fita_%d.txt", fitaNumero);

    fitaFinal = fopen(nomeFitaFinal, "r"); // Abre a fita correta
    arquivoFinal = fopen("provao.TXT", "w");

    if (!fitaFinal || !arquivoFinal)
    {
        printf("\nErro ao abrir a fita final ou o arquivo provao.TXT\n");
        fclose(arquivo); // Fecha o arquivo aberto anteriormente
        return;
    }

    // Copiar registros para provao
    while (contador < quantidade && fgets(linha, sizeof(linha), fitaFinal))
    {
        fputs(linha, arquivoFinal);
        contador++;
    }

    fclose(fitaFinal);
    fclose(arquivoFinal);
    fclose(arquivo);

    printf("Dados ordenados copiados para o arquivo provao.TXT a partir da fita %d.\n", fitaNumero);
}