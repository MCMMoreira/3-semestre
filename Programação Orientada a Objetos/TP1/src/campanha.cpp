#include "include/campanha.h"
#include <iomanip>
using namespace std;

void Campanha :: setBatalha (Batalha* b)
{
    batalhas.push_back(b);
}
Batalha* Campanha :: getBatalha (int index)
{
    return batalhas[index];
}

//Libera o vector de batalhas
Campanha :: ~Campanha() {
    for (Batalha* batalha : batalhas) {
        delete batalha;
    }

    batalhas.clear();
}

void Campanha :: simularBatalhas()
{
    Exercito *A, *B;
    vector <Unidade*> unidadesA, unidadesB;

    for (size_t i = 0; i<batalhas.size(); i++)
    {
        A = batalhas[i]->getExercitoA();
        B = batalhas[i]->getExercitoB();

        int somaPoderAtaqueA = 0, somaPoderAtaqueB = 0;
        //monta as unidades do exercito A e B

        for (int i = 0; i<UNIDADES; i++)
        {
            //inicializa os atributos com valores aleatorios a cada batalha
            A->setUnidade(i);
            B->setUnidade(i);
        }

        unidadesA = A->getUnidades();
        unidadesB = B->getUnidades();

        for (int i = 0; i<UNIDADES; i++)
        {
            //calcula o poder de ataque total dos exercitos A e B
            somaPoderAtaqueA += unidadesA[i]->getPoderAtaque();
            somaPoderAtaqueB += unidadesB[i]->getPoderAtaque();
        }
        
        if (somaPoderAtaqueA > somaPoderAtaqueB)
        {
            //Se o exercito A vencer, ele realiza um ataque e incrementa o numero de vitorias de A e o numero de derrotas de B
            batalhas[i]->ataqueExercitoA();
            A->setVitoria();
            B->setDerrota();
        }

        else if (somaPoderAtaqueA < somaPoderAtaqueB)
        {
            //Se o exercito B vencer, ele realiza um ataque e incrementa o numero de vitorias de B e o numero de derrotas de A
            batalhas[i]->ataqueExercitoB();
            B->setVitoria();
            A->setDerrota();
        }

        else
        {
            //Se empatarem, ambos realizam um ataque e o numero de empates e incrementado em ambos os exercitos
            batalhas[i]->ataqueExercitoA();
            batalhas[i]->ataqueExercitoB();
            A->setEmpate();
            B->setEmpate();
        }
    }
}
void Campanha :: gerarTabelaDePosicoes(vector <Exercito*> tropas)
{
    size_t index;
    Exercito *temp;

    //ordena os exercitos pelo número de vitorias
    for (size_t i = 0; i<tropas.size()-1; i++)
    {
        index = i;

        for (size_t j = i+1; j<tropas.size(); j++)
        {
            if (tropas[j]->getVitoria() > tropas[index]->getVitoria())
            {
                index = j;
            }
        }

        if (index != i)
        {
            temp = tropas[i];
            tropas[i] = tropas[index];
            tropas[index] = temp;
        }
    }


    cout << "\n\n|                   Tabela de Posicoes                   |\n";
    cout << "|--------------------------------------------------------|\n";
    cout << "|   Exercito     |    Vitorias   |  Empates  |  Derrotas |\n";

    for (size_t i = 0; i<tropas.size(); i++)
    {
        cout << "| " << setw(15) << left << tropas[i]->getNome() 
             << "| " << setw(14) << left << tropas[i]->getVitoria() 
             << "| " << setw(10) << left << tropas[i]->getEmpate() 
             << "| " << setw(10) << left << tropas[i]->getDerrota() 
             << "|\n";
    }
    
    cout << "|--------------------------------------------------------|\n";

}
void Campanha :: mostrarUnidadeMaisDestrutiva(vector <Exercito*> tropas)
{
    int maior = 0, index = 0;
    string nome;
    
    vector <Unidade*> unidades;

    for (size_t i = 0; i<tropas.size(); i++)
    {
        for (int j = 0; j<UNIDADES; j++)
        {
            unidades = tropas[i]->getUnidades();

            if (unidades[j]->getDestruicoes() > maior)
            {
                maior = unidades[j]->getDestruicoes(); //encontra o maior numero de destruicoes
                nome = tropas[i]->getNome();
                index = i; //armazena a posicao da unidade dentro do vetor
            }
        }
    }

    cout << "\nA unidade mais destrutiva é " << unidades[index]->getNome() << " do exército " << nome << " com " << maior << " destruicoes\n";

    for (Batalha* batalha : batalhas) {
        delete batalha;
    }

    batalhas.clear();
}
