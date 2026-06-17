#ifndef INFANTARIA_H
#define INFANTARIA_H
#include "unidade.h"

class Infantaria : public Unidade{
    int forca;
    int velocidade;
    string nome;

public:
    Infantaria(int = 0, int = 0, int = 0, int = 0, string = "uma infantaria");
    void setAtributos();
    string getNome()const;
    int getPoderAtaque();
};

#endif
