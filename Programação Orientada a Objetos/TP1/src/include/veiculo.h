#ifndef VEICULO_H
#define VEICULO_H
#include "unidade.h"

class Veiculo : public Unidade{
    int blindagem;
    int potenciaDeFogo;
    string nome;

public:
    Veiculo(int = 0, int = 0, int = 0, int = 0, string = "um veículo");
    void setAtributos();
    int getPoderAtaque();
    string getNome()const;
};

#endif
