#ifndef CAMPANHA_H
#define CAMPANHA_H

#include <vector>
#include "batalha.h"
#include "aeronave.h"
#include "veiculo.h"
#include "infantaria.h"

#define NUM_BATALHAS 5 //numero de batalhas entre os dois exercitos
#define AERO 1
#define VEIC 2
#define INFAN 2
#define UNIDADES (AERO+VEIC+INFAN)

class Campanha{
    vector <Batalha*> batalhas;

public:
    void setBatalha (Batalha* b);
    Batalha* getBatalha (int index);
    virtual ~Campanha();
    void simularBatalhas();
    void gerarTabelaDePosicoes(vector <Exercito*>);
    void mostrarUnidadeMaisDestrutiva(vector <Exercito*>);
};

#endif
