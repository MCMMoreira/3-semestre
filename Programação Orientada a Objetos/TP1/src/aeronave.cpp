#include <iostream>
#include "include/aeronave.h"
#include <cstdlib>
#include <ctime> 
using namespace std;

Aeronave::Aeronave(int poder, int destruicoes, int m, int a, string n): Unidade(poder,destruicoes),
manobrabilidade(m), alcance(a), nome(n){}

void Aeronave::setAtributos(){
    this->poderDeAtaque = 50 + rand() % 41;     
    this->manobrabilidade = 30 + rand() % 41;      
    this->alcance = 20 + rand() % 31;
}

string Aeronave::getNome()const{
    return nome;
}

int Aeronave::getPoderAtaque(){
    return (poderDeAtaque*5 + manobrabilidade*3 + alcance*2)/10;
}
