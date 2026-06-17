#include <iostream>
#include "include/veiculo.h"
#include <cstdlib>
#include <ctime> 
using namespace std;

Veiculo::Veiculo(int poder, int destruicoes, int blind, int potFogo, string n): Unidade(poder,destruicoes),
blindagem(blind), potenciaDeFogo(potFogo), nome(n){}

void Veiculo::setAtributos(){
    this->poderDeAtaque = 40 + rand() % 41;     
    this->blindagem = 30 + rand() % 41;      
    this->potenciaDeFogo = 20 + rand() % 31;
}

string Veiculo::getNome()const{
    return nome;
}

int Veiculo::getPoderAtaque(){
    return (poderDeAtaque*5 + blindagem*3 + potenciaDeFogo*2)/10;
}
