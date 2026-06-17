#include <iostream>
#include <sstream>
#include "include/exercito.h"

Exercito::Exercito(string nome, int vitorias, int derrotas, int empates):nome(nome),vitorias(vitorias),
derrotas(derrotas),empates(empates){}

ostream& operator <<(ostream& out, Exercito& ex){
    out << "Vitórias: " << ex.vitorias 
       << ", Empates: " << ex.empates 
       << ", Derrotas: " << ex.derrotas;
    return out;
}

void Exercito::adicionarUnidade(Unidade* unidade) {
    unidades.push_back(unidade);
}

string Exercito::getResultados() {
    ostringstream resultado;
    resultado << *this; // Usa a sobrecarga do operador << para evitar repetição de código
    return resultado.str();
}

string Exercito :: getNome()
{
    return nome;
}

std::vector<Unidade*>& Exercito::getUnidades() 
{
    return unidades;
}

void Exercito :: setVitoria()
{
    vitorias++;
}
void Exercito :: setDerrota()
{
    derrotas++;
}
void Exercito :: setEmpate()
{
    empates++;
}

void Exercito :: setUnidade(int i)
{
    unidades[i]->setAtributos();
}

int Exercito::getVitoria()const{
    return vitorias;
}

int Exercito::getDerrota()const{
    return derrotas;
}

int Exercito::getEmpate()const{
    return empates;
}

void Exercito::imprimeUnidades(){
    for(Unidade* unidade : unidades){
        cout << "\n\nNome: " << unidade->getNome() << "\nPoder de ataque: " << unidade->getPoderAtaque();
    }
}

Exercito::~Exercito() {
    for (Unidade* unidade : unidades) {
        delete unidade;
    }

    unidades.clear();
}
