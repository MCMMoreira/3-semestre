#ifndef AERONAVE_H
#define AERONAVE_H
#include "unidade.h"

class Aeronave : public Unidade{
    int manobrabilidade;
    int alcance;
    string nome;

public:
    Aeronave(int = 0, int = 0, int = 0, int = 0, string = "uma aeronave");
    void setAtributos();
    int getPoderAtaque();
    string getNome()const;
};

#endif
