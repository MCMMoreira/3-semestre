#ifndef BATALHA_H
#define BATALHA_H

#include "exercito.h"
#include "date.h"

class Batalha {

    Date data;
    Exercito* exercitoA;
    Exercito* exercitoB;
    int resultadoA;
    int resultadoB;

public:
   
    Batalha(Date, Exercito*, Exercito*);
    Exercito* getExercitoA();
    Exercito* getExercitoB();
    void ataqueExercitoA();
    void ataqueExercitoB();

    std::string getResultado() const;
    
};

#endif
