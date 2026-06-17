#include "include/batalha.h"
#include <iostream>
#include <sstream>

Batalha::Batalha(Date d, Exercito* a, Exercito* b) 
    : data(d), exercitoA(a), exercitoB(b), resultadoA(0), resultadoB(0) {}

Exercito* Batalha :: getExercitoA()
{
    return exercitoA;
}
Exercito* Batalha :: getExercitoB()
{
    return exercitoB;
}

void Batalha::ataqueExercitoA() {
    vector<Unidade*>& unidadesA = exercitoA->getUnidades();
    vector<Unidade*>& unidadesB = exercitoB->getUnidades();

    for (size_t i = 0; i < unidadesA.size() && i < unidadesB.size(); i++) {
        Unidade* unidadeA = unidadesA[i];
        Unidade* unidadeB = unidadesB[i];

        if (unidadeA->getPoderAtaque() > unidadeB->getPoderAtaque()) {
            resultadoA++;
            unidadeA->somaDestruicao();
        }
    }
}

void Batalha::ataqueExercitoB() {
    vector<Unidade*>& unidadesA = exercitoA->getUnidades();
    vector<Unidade*>& unidadesB = exercitoB->getUnidades();

    for (size_t i = 0; i < unidadesA.size() && i < unidadesB.size(); i++) {
        Unidade* unidadeA = unidadesA[i];
        Unidade* unidadeB = unidadesB[i];

        if (unidadeB->getPoderAtaque() > unidadeA->getPoderAtaque()) {
            resultadoB++;
            unidadeB->somaDestruicao();
        }
    }
}

std::string Batalha::getResultado() const {
    std::ostringstream oss;
    oss << exercitoA->getNome() << " " << resultadoA 
        << " x " << resultadoB << " " << exercitoB->getNome();
    return oss.str();
}


