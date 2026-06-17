#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/inter2f.h"
#include "include/interf+1.h"
#include "include/selecao.h"
#include "include/funcoes.h"

#define NUM_FITAS 10
#define TAM_LINHA 256

int intercalarFitasOrdenadas2F(Resultados* resultados)
{
    FILE *fitasEntrada[NUM_FITAS];
    FILE *fitaSaida[NUM_FITAS];
    char nomeArquivo[30];
    Aluno *blocosOrdenados[NUM_FITAS];
    int numRegistros[NUM_FITAS];
    int indices[NUM_FITAS] = {0};

    bool finalizado = false; // Indica quando o processo de intercalação está concluído
    int iteracao = 0;       // Contador de iterações (para alternar entre fitas de entrada e saída)

    while (!finalizado) {
        // Define os números das fitas de entrada e saída com base no valor de iteracao
        int inicioEntrada = (iteracao % 2 == 0) ? 1 : 11;
        int inicioSaida = (iteracao % 2 == 0) ? 11 : 1;

        // Abre as fitas de entrada
        for (int i = 0; i < NUM_FITAS; i++) {
            snprintf(nomeArquivo, sizeof(nomeArquivo), "Fitas/fita_%d.txt", inicioEntrada + i);
            fitasEntrada[i] = fopen(nomeArquivo, "r");
            if (!fitasEntrada[i]) {
                printf("Erro ao abrir a fita de entrada %d\n", inicioEntrada + i);
                for (int j = 0; j < NUM_FITAS; j++) {
                    if (blocosOrdenados[j]) free(blocosOrdenados[j]);
                }
                return 1;
            }
        }

        // Abre as fitas de saída
        for (int i = 0; i < NUM_FITAS; i++) {
            snprintf(nomeArquivo, sizeof(nomeArquivo), "Fitas/fita_%d.txt", inicioSaida + i);
            fitaSaida[i] = fopen(nomeArquivo, "w");
            if (!fitaSaida[i]) {
                printf("Erro ao abrir a fita de saída %d\n", inicioSaida + i);
                for (int j = 0; j < NUM_FITAS; j++) {
                    if (blocosOrdenados[j]) free(blocosOrdenados[j]);
                    if (fitasEntrada[j]) fclose(fitasEntrada[j]);
                }
                return 1;
            }
        }

        // Alocação de memória para os blocos ordenados
        for (int i = 0; i < NUM_FITAS; i++) {
            blocosOrdenados[i] = malloc(sizeof(Aluno) * 100); // Assumindo no máximo 100 registros por bloco
            if (blocosOrdenados[i] == NULL) {
                printf("Erro ao alocar memória\n");
                for (int j = 0; j < i; j++) {
                    free(blocosOrdenados[j]);
                }
                for (int j = 0; j < NUM_FITAS; j++) {
                    if (fitasEntrada[j]) fclose(fitasEntrada[j]);
                    if (fitaSaida[j]) fclose(fitaSaida[j]);
                }
                return 1;
            }
        }

        bool fitasAtivas = true; // Indica se ainda há blocos para intercalar
        int fitaSaidaAtual = 0; // Controla para qual fita de saída escrever

        // Intercala registros das fitas de entrada
        while (fitasAtivas) {

            fitasAtivas = false;

            // Lê um bloco de cada fita de entrada
            for (int i = 0; i < NUM_FITAS; i++) {
                if (fitasEntrada[i]) {
                    numRegistros[i] = lerBloco(fitasEntrada[i], &blocosOrdenados[i], resultados);
                    indices[i] = 0;
                    if (numRegistros[i] > 0) fitasAtivas = true; // Ainda há dados para processar
                }
            }

            // Intercala os registros dos blocos lidos
            while (true) {
                
                int minIndex = -1;

                // Encontra o registro com a menor nota entre os blocos
                for (int i = 0; i < NUM_FITAS; i++) {
                    if (indices[i] < numRegistros[i]) {
                        if (minIndex == -1 || blocosOrdenados[i][indices[i]].nota < blocosOrdenados[minIndex][indices[minIndex]].nota) {
                            minIndex = i;
                            resultados->ordenacao.comparacoes++; // Contabiliza a comparação
                        }
                    }
                }

                // Se não houver mais registros, termina o loop
                if (minIndex == -1) break;

                // Escreve o registro na fita de saída atual
                fprintf(fitaSaida[fitaSaidaAtual], "%08ld %.1f %-2s %-50s %-30s\n",
                        blocosOrdenados[minIndex][indices[minIndex]].inscricao,
                        blocosOrdenados[minIndex][indices[minIndex]].nota,
                        blocosOrdenados[minIndex][indices[minIndex]].estado,
                        blocosOrdenados[minIndex][indices[minIndex]].cidade,
                        blocosOrdenados[minIndex][indices[minIndex]].curso);

                resultados->ordenacao.escrita++; // Contabiliza a escrita
                indices[minIndex]++; // Avança para o próximo registro do bloco
            }

            // Escreve uma linha em branco para separar os blocos
            fprintf(fitaSaida[fitaSaidaAtual], "\n");

            // Avança para a próxima fita de saída (rotaciona entre as fitas de saída)
            fitaSaidaAtual = (fitaSaidaAtual + 1) % NUM_FITAS;
        }

        // Fecha as fitas de entrada e libera a memória alocada
        for (int i = 0; i < NUM_FITAS; i++) {
            fclose(fitasEntrada[i]);
            free(blocosOrdenados[i]);
        }

        // Fecha as fitas de saída
        for (int i = 0; i < NUM_FITAS; i++) {
            fclose(fitaSaida[i]);
        }

        // Verifica se o processo está finalizado (apenas uma fita de saída contém dados)
        int numBlocos = 0;
        for (int i = 0; i < NUM_FITAS; i++) {
            snprintf(nomeArquivo, sizeof(nomeArquivo), "Fitas/fita_%d.txt", inicioSaida + i);
            FILE *fita = fopen(nomeArquivo, "r");
            if (fita) {
                char linha[TAM_LINHA];
                if (fgets(linha, sizeof(linha), fita) != NULL) {
                    numBlocos++;
                }
                fclose(fita);
            }
        }

        // Se houver apenas uma fita com dados, o processo está finalizado
        if (numBlocos <= 2) {
            finalizado = true;
            return iteracao;
        }

        iteracao++; // Incrementa o contador de iterações
    }

    return 1;
}
