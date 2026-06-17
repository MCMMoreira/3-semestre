#ifndef INTERCALACAO_H
#define INTERCALACAO_H

#include "funcoes.h"

void gerarfitaSaida(FILE **arquivoSaida);
int lerBloco(FILE *fita, Aluno **registros, Resultados* resultados);
void intercalarFitasOrdenadas(Resultados* resultados);
int redistribuirBlocos(Resultados* resultados);
void ordenarFitas(Resultados* resultados); 

#endif
