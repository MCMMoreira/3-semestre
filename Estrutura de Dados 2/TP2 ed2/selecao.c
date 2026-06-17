#include "include/selecao.h"
#include "include/funcoes.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TamVect 19
#define NUM_FITAS 19

void heaprefaz(RegistroParaSubstituicao* registros, int n, int i, Resultados* resultados) 
{
    int menor = i;  
    int esq = 2 * i + 1; // Filho esquerdo
    int dir = 2 * i + 2; // Filho direito

    resultados->selecao.comparacoes++;
    if (esq < n && registros[esq].registro.nota < registros[menor].registro.nota)
        menor = esq;

    resultados->selecao.comparacoes++;
    if (dir < n && registros[dir].registro.nota < registros[menor].registro.nota)
        menor = dir;

    if (menor != i) {
        RegistroParaSubstituicao temp = registros[i];
        registros[i] = registros[menor];
        registros[menor] = temp;

        heaprefaz(registros, n, menor, resultados);
    }
}

void construirHeap(RegistroParaSubstituicao* registros, int n, Resultados* resultados) 
{
    for (int i = n / 2 - 1; i >= 0; i--) {
        heaprefaz(registros, n, i, resultados);
    }
}

RegistroParaSubstituicao extrairMin(RegistroParaSubstituicao* registros, int* n, Resultados* resultados) 
{
    int i = 0;
    while (i < *n && registros[i].marcado) {
        i++;
    }

    if (i == *n) {
        RegistroParaSubstituicao invalido;
        invalido.marcado = true;
        return invalido;
    }

    int menorIdx = i;
    for (int j = i + 1; j < *n; j++) {
        resultados->selecao.comparacoes++;
        if (!registros[j].marcado && registros[j].registro.nota < registros[menorIdx].registro.nota) {
            menorIdx = j;
        }
    }

    RegistroParaSubstituicao menor = registros[menorIdx];
    registros[menorIdx] = registros[*n - 1];
    (*n)--;

    construirHeap(registros, *n, resultados);

    return menor;
}

bool todosMarcados(RegistroParaSubstituicao* registros, int n) 
{
    if (n == 0) return false; 

    for (int i = 0; i < n; i++) {
        if (!registros[i].marcado) {
            return false; 
        }
    }
    return true; 
}

void desmarcarRegistros(RegistroParaSubstituicao* registros, int n) 
{
    for (int i = 0; i < n; i++) {
        if (registros[i].marcado) {
            registros[i].marcado = false; 
        }
    }
}

void selecaoPorSubstituicao(FILE *arquivoEntrada, int quantidade, Resultados* resultados) 
{
    RegistroParaSubstituicao blocoPorSubstituicao[TamVect];
    int numRegistrosLidos = 0;
    int indiceFita = 0;
    FILE *fitas[NUM_FITAS];
    int contadorLeituras = 0;  // Contador de leituras

    // Abre os arquivos de fita para escrita
    for (int i = 0; i < NUM_FITAS; i++) {
        char nomeArquivo[20];
        sprintf(nomeArquivo, "Fitas/fita_%d.txt", i + 1);
        fitas[i] = fopen(nomeArquivo, "w");
        if (fitas[i] == NULL) {
            printf("Erro ao criar arquivo de fita %d.\n", i + 1);
            return;
        }
    }

    // Leitura inicial do arquivo
    while (numRegistrosLidos < TamVect && contadorLeituras < quantidade) {
        if (fscanf(arquivoEntrada, "%08ld %lf %2s %50[^\n] %30[^\n]", 
                   &blocoPorSubstituicao[numRegistrosLidos].registro.inscricao,
                   &blocoPorSubstituicao[numRegistrosLidos].registro.nota,
                   blocoPorSubstituicao[numRegistrosLidos].registro.estado,
                   blocoPorSubstituicao[numRegistrosLidos].registro.cidade,
                   blocoPorSubstituicao[numRegistrosLidos].registro.curso) == 5) {
            blocoPorSubstituicao[numRegistrosLidos].marcado = false;
            numRegistrosLidos++;
            contadorLeituras++;  // Incrementa o contador de leituras
            resultados->selecao.leitura++;
        } else {
            break;  // Se a leitura falhar (menos de 5 elementos lidos), sai do loop
        }
    }

    if (numRegistrosLidos == 0) {
        printf("Arquivo vazio ou não foi possível ler registros.\n");
        return;
    }

    construirHeap(blocoPorSubstituicao, numRegistrosLidos, resultados);

    int numItensBloco = 0;

    // Loop principal para gravar nas fitas
    while (!todosMarcados(blocoPorSubstituicao, numRegistrosLidos) && contadorLeituras < quantidade) {  
        // Extrai o menor registro
        RegistroParaSubstituicao Registro = extrairMin(blocoPorSubstituicao, &numRegistrosLidos, resultados);
        RegistroParaSubstituicao aux = Registro;

        // Escreve o menor registro em formato texto na fita
        fprintf(fitas[indiceFita], "%08ld %.1f %-2s %-50s %-30s\n",
                Registro.registro.inscricao,
                Registro.registro.nota,
                Registro.registro.estado,
                Registro.registro.cidade,
                Registro.registro.curso);

        numItensBloco++;
        resultados->selecao.escrita++; 

        // Lê um novo registro e realiza substituição no bloco
        if (fscanf(arquivoEntrada, "%08ld %lf %2s %50[^\n] %30[^\n]", 
                   &Registro.registro.inscricao,
                   &Registro.registro.nota,
                   Registro.registro.estado,
                   Registro.registro.cidade,
                   Registro.registro.curso) == 5) {

            resultados->selecao.comparacoes++; 
            if (Registro.registro.nota < aux.registro.nota) {
                Registro.marcado = true;
            } else {
                Registro.marcado = false;
            }
            blocoPorSubstituicao[numRegistrosLidos] = Registro;
            numRegistrosLidos++;
            contadorLeituras++; 
            resultados->selecao.leitura++; 
            heaprefaz(blocoPorSubstituicao, numRegistrosLidos, 0, resultados);
        }

        // Verifica se todos os registros do bloco estão marcados
        if (todosMarcados(blocoPorSubstituicao, numRegistrosLidos)) {
            fprintf(fitas[indiceFita], "\n");
            desmarcarRegistros(blocoPorSubstituicao, numRegistrosLidos);
            numItensBloco = 0;
            indiceFita = (indiceFita + 1) % NUM_FITAS;
        }
    }

    // Loop para garantir que o heap seja completamente esvaziado
    while (numRegistrosLidos > 0) {
        
        RegistroParaSubstituicao Registro = extrairMin(blocoPorSubstituicao, &numRegistrosLidos, resultados);
        
        // Escreve o menor registro na fita
        fprintf(fitas[indiceFita], "%08ld %.1f %-2s %-50s %-30s\n",
                Registro.registro.inscricao,
                Registro.registro.nota,
                Registro.registro.estado,
                Registro.registro.cidade,
                Registro.registro.curso);
                resultados->selecao.escrita++; 

        // Verifica se todos os registros do vetor estão marcados
        if (todosMarcados(blocoPorSubstituicao, numRegistrosLidos)) {
            fprintf(fitas[indiceFita], "\n");
            desmarcarRegistros(blocoPorSubstituicao, numRegistrosLidos);
            numItensBloco = 0;
            indiceFita = (indiceFita + 1) % NUM_FITAS;
        }
    }

    // Fecha os arquivos de fita
    for (int i = 0; i < NUM_FITAS; i++) {
        fclose(fitas[i]);
    }

    // Exibir o número total de leituras realizadas
    printf("Total de leituras realizadas: %d\n", contadorLeituras);
}
