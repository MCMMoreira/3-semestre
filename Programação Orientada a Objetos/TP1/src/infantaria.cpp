#include <iostream>
#include "include/infantaria.h"
#include <cstdlib>
#include <ctime> 
using namespace std;

Infantaria::Infantaria(int poder, int destruicoes, int forca, int velocidade, string nome): Unidade(poder,destruicoes),
forca(forca), velocidade(velocidade), nome(nome){}

void Infantaria::setAtributos(){
    this->poderDeAtaque = 30 + rand() % 31;     
    this->forca = 20 + rand() % 31;      
    this->velocidade = 10 + rand() % 31;
}

string Infantaria::getNome()const{
    return nome;
}

int Infantaria::getPoderAtaque(){
    return (poderDeAtaque*5 + forca*3 + velocidade*2)/10;
}
