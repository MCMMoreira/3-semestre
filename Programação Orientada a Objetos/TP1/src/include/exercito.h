#ifndef EXERCITO_H
#define EXERCITO_H

#include <iostream>
#include <vector>
#include "unidade.h"

using namespace std;

class Exercito{
    string nome;
    int vitorias;
    int derrotas;
    int empates;
    vector<Unidade*> unidades;

public:
    Exercito(string="",int = 0, int = 0, int = 0);
    virtual ~Exercito();
    void adicionarUnidade(Unidade*);
    string getResultados();
    string getNome();
    void imprimeUnidades();
    void setVitoria();
    void setDerrota();
    void setEmpate();
    void setUnidade(int i);
    int getVitoria()const;
    int getDerrota()const;
    int getEmpate()const;
    std::vector<Unidade*>& getUnidades();

    friend ostream& operator <<(ostream& out, Exercito& ex);
};

#endif
