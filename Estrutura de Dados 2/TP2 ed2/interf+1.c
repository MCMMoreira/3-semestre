#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/interf+1.h"
#include "include/selecao.h"
#include "include/funcoes.h"

#define NUM_FITAS 19
#define TAM_LINHA 256

Resultados resultados;

void gerarfitaSaida(FILE **arquivoSaida) 
{
    char nomeArquivo[30];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "Fitas/fita_20.txt");
    *arquivoSaida = fopen(nomeArquivo, "w");
    if (*arquivoSaida == NULL) {
        printf("Erro ao criar o arquivo.\n");
    }
}

int lerBloco(FILE *fita, Aluno **registros, Resultados* resultados) 
{
    int count = 0;
    char linha[TAM_LINHA];
    
    *registros = malloc(sizeof(Aluno) * 100);
    if (*registros == NULL) return 0;
    
    while (fgets(linha, sizeof(linha), fita) != NULL) {
        resultados->ordenacao.leitura++;
        if (linha[0] == '\n') break;
        
        if (count >= 100) {
            Aluno *temp = realloc(*registros, sizeof(Aluno) * (count + 100));
            if (temp == NULL) {
                free(*registros);
                *registros = NULL;
                return count;
            }
            *registros = temp;
        }

        sscanf(linha, "%ld %lf %2s %50[^\"\n] %30[^\n]",
               &(*registros)[count].inscricao, &(*registros)[count].nota,
               (*registros)[count].estado, (*registros)[count].cidade,
               (*registros)[count].curso);
        count++;
    }
    return count;
}

void intercalarFitasOrdenadas(Resultados* resultados) 
{

    FILE *fitasEntrada[NUM_FITAS], *fitaSaida; 
    char nomeArquivo[30]; 
    Aluno *blocosOrdenados[NUM_FITAS]; 
    int numRegistros[NUM_FITAS]; 
    int indices[NUM_FITAS] = {0}; 

    for (int i = 0; i < NUM_FITAS; i++) {
        blocosOrdenados[i] = malloc(sizeof(Aluno) * 100); 

        if (blocosOrdenados[i] == NULL) {
            printf("Erro ao alocar memória\n");

            for (int j = 0; j < i; j++) {
                free(blocosOrdenados[j]);
            }
            return; 
        }
    }

    //arquivos de entrada
    for (int i = 0; i < NUM_FITAS; i++) {
        snprintf(nomeArquivo, sizeof(nomeArquivo), "Fitas/fita_%d.txt", i + 1); 
        fitasEntrada[i] = fopen(nomeArquivo, "r"); 

        if (!fitasEntrada[i]) {
            printf("Erro ao abrir a fita %d\n", i + 1);
    
            for (int j = 0; j <= i; j++) {
                free(blocosOrdenados[j]);
            }
            return; 
        }
    }

    //arquivo de saída
    fitaSaida = fopen("Fitas/fita_20.txt", "w"); 
    if (!fitaSaida) {
        printf("Erro ao criar a fita de saída.\n");

        for (int i = 0; i < NUM_FITAS; i++) {
            fclose(fitasEntrada[i]);
            free(blocosOrdenados[i]);
        }
        return; 
    }

    // Intercalação dos blocos 
    bool fitasAtivas = true;
    while (fitasAtivas) {
        fitasAtivas = false; 

        //Leitura dos blocos
        for (int i = 0; i < NUM_FITAS; i++) {
            if (fitasEntrada[i]) {
                
                numRegistros[i] = lerBloco(fitasEntrada[i], &blocosOrdenados[i], resultados);
                indices[i] = 0; 
                if (numRegistros[i] > 0) fitasAtivas = true; 
            }
        }

        //Intercalação dos registros 
        while (true) {
            int minIndex = -1; 

            // Encontra o menor registro
            for (int i = 0; i < NUM_FITAS; i++) {
                if (indices[i] < numRegistros[i]) { 
                    if (minIndex == -1 || blocosOrdenados[i][indices[i]].nota < blocosOrdenados[minIndex][indices[minIndex]].nota) {
                        minIndex = i; 
                        resultados->ordenacao.comparacoes++; 
                    }
                }
            }

            if (minIndex == -1) break;

            // Escreve o menor registro
            fprintf(fitaSaida, "%08ld %.1f %-2s %-50s %-30s\n",
                    blocosOrdenados[minIndex][indices[minIndex]].inscricao,
                    blocosOrdenados[minIndex][indices[minIndex]].nota,
                    blocosOrdenados[minIndex][indices[minIndex]].estado,
                    blocosOrdenados[minIndex][indices[minIndex]].cidade,
                    blocosOrdenados[minIndex][indices[minIndex]].curso);
            
            resultados->ordenacao.escrita++; 
            indices[minIndex]++; 
        }
        fprintf(fitaSaida, "\n"); 
    }

    fclose(fitaSaida); 
    for (int i = 0; i < NUM_FITAS; i++) {
        fclose(fitasEntrada[i]);
        free(blocosOrdenados[i]);
    }
}

int redistribuirBlocos(Resultados* resultados) 
{
    FILE *fitaSaida, *fitasEntrada[NUM_FITAS];
    char nomeArquivo[30];
    char linha[256];
    int fitaAtual = 0;
    int numBlocos = 0; 
    bool blocoCompleto = false;

    fitaSaida = fopen("Fitas/fita_20.txt", "r");
    if (!fitaSaida) {
        perror("Erro ao abrir fita de saída");
        return 0;
    }

    // Abre as fitas de entrada para escrita
    for (int i = 0; i < NUM_FITAS; i++) {
        snprintf(nomeArquivo, sizeof(nomeArquivo), "Fitas/fita_%d.txt", i + 1);
        fitasEntrada[i] = fopen(nomeArquivo, "w");
        if (!fitasEntrada[i]) {
            perror("Erro ao abrir fita de entrada");
            fclose(fitaSaida);
            return 0;
        }
    }

    // Redistribui os blocos de forma rotativa
    while (fgets(linha, sizeof(linha), fitaSaida) != NULL) {
        resultados->ordenacao.leitura++;

        if (linha[0] == '\n') {
            //escrever a linha em branco no final do bloco
            if (blocoCompleto) {
                fputs("\n", fitasEntrada[fitaAtual]); 
            }
            fitaAtual = (fitaAtual + 1) % NUM_FITAS; 
            numBlocos++; 
            blocoCompleto = false; 
        } else {
            
            fputs(linha, fitasEntrada[fitaAtual]);
            blocoCompleto = true; 
        }
    }

    // Verifique se o último bloco também deve terminar com uma linha em branco
    if (blocoCompleto) {
        fputs("\n", fitasEntrada[fitaAtual]);
        numBlocos++;
        resultados->ordenacao.escrita++; 
    }

    fclose(fitaSaida);
    for (int i = 0; i < NUM_FITAS; i++) {
        fclose(fitasEntrada[i]);
    }

    return numBlocos; 
}

void ordenarFitas(Resultados* resultados) 
{
    int numBlocos = -1; 

    while (numBlocos != 2) {  
        intercalarFitasOrdenadas(resultados); 
        numBlocos = redistribuirBlocos(resultados);
    }

    printf("Ordenação concluída!\n");
}

