#ifndef UNIDADE_H
#define UNIDADE_H

#include <iostream>
using namespace std;

class Unidade{
protected:
    int poderDeAtaque;
    int destruicoes;

public:
    virtual ~Unidade() {}
    Unidade(int = 0, int = 0);
    virtual int getPoderAtaque() = 0;
    virtual void setAtributos() = 0;
    virtual int getDestruicoes();
    virtual string getNome() const = 0;
    void somaDestruicao();
};

#endif
