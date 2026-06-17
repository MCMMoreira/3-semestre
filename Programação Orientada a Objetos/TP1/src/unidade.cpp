#include "include/unidade.h"

Unidade :: Unidade (int p, int d)
{
    poderDeAtaque = p;
    destruicoes = d;
}

int Unidade :: getDestruicoes()
{
    return destruicoes;
}

void Unidade :: somaDestruicao()
{
    destruicoes++;
}
