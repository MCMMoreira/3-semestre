#include "include/unidade.h"
#include "include/aeronave.h"
#include "include/infantaria.h"
#include "include/veiculo.h"
#include "include/exercito.h"
#include "include/date.h"
#include "include/batalha.h"
#include <time.h>
#include "include/campanha.h"

#include <iostream>
using namespace std;

vector<Exercito*> criaJogadores();
void iniciaBatalha(vector<Exercito*> tropas);
void criaExercito(vector<Exercito*> &tropas);
void imprimeExercito(vector<Exercito*> tropas);
void competicao (vector <Exercito*> tropas, Campanha *camp);

int main()
{
    vector <Exercito*> tropas;
    Campanha *camp = new Campanha();
    int opcao = 0;
    string nome;

    srand(time(0));

    cout << "\n       Bem-Vindo ao Jogo!       ";
    cout << "\n--------------------------------";
    
    tropas = criaJogadores();

    cout << "\n--------------------------------";
    cout << "\n   Escolha uma opcao do menu:   ";
    cout << "\n--------------------------------";
    cout <<"\n1- Iniciar uma batalha isolada\n2- Criar um novo exercito\n3- Ver caracteristicas do exercito\n4- Simular Campanha\n5- Sair\n\n";

    while (opcao != 5)
    {
        cin >> opcao;
        
        switch (opcao)
        {
            case 1:
                iniciaBatalha(tropas);
                break;
            
            case 2:
                criaExercito(tropas);
                break;
            
            case 3: 
                imprimeExercito(tropas);
                break;
            
            case 4:
                competicao (tropas, camp);
                break;

            case 5:
                break;
            
            default:
                cout << "\nOpcao Invalida!\n";
        }

        cout << "\n--------------------------------";
        cout << "\n   Escolha uma opcao do menu:   ";
        cout << "\n--------------------------------";
        cout <<"\n1- Iniciar uma batalha isolada\n2- Criar um novo exercito\n3- Ver caracteristicas do exercito\n4- Simular Campanha\n5- Sair\n\n";
    }

    for (Exercito* ex : tropas) {
        delete ex;
    }

    tropas.clear();

    delete camp;

    return 0;
}

vector<Exercito*> criaJogadores()
{
    vector <Exercito*> tropas;
    string nome;
    int jogadores;

    cout << "\nAntes de começar, informe a quantidade de jogadores para criarmos os exércitos!\n";
    
    try{
        if(!(cin >> jogadores)) throw runtime_error("");
        for (int i = 0; i<jogadores; i++)
        {
            cout << "\nDigite o nome do exército " << i+1 << ": ";
            cin >> nome;

            Exercito* ex = new Exercito (nome);

            for (int j = 0; j< AERO; j++)
            {
                Aeronave *a = new Aeronave();
                ex->adicionarUnidade(a);
            }

            for (int j = 0; j< INFAN; j++)
            {
                Infantaria *i = new Infantaria();
                ex->adicionarUnidade(i);
            }

            for (int j = 0; j< VEIC; j++)
            {
                Veiculo *v = new Veiculo();
                ex->adicionarUnidade(v);
            }

            tropas.push_back(ex);
        }

    }
    catch(bad_alloc& e){
        cout << "\nErro de alocacao\n";
    }

    catch(...){
        cout << "Tipo incorreto na leitura ";
    }
    
    cout << "\nÓtimo! Exércitos criados!\n";

    return tropas;
}

void iniciaBatalha(vector<Exercito*> tropas)
{
    string nome;
    int d, m, a, ataque, flag = 0;

    Exercito* tropa1;
    Exercito* tropa2;

    cout << "\nPara criar uma batalha, e necessario escolher dois exercitos...";
    cout << "\nDigite o nome do primeiro exercito: ";
    cin >> nome;

    for (size_t i = 0; i<tropas.size(); i++)
    {
        if (tropas[i]->getNome() == nome)
        {
            tropa1 = tropas[i];
            flag = 1;
            break;
        } 
    }

    if (flag == 0)
    {
        cout << "\nNão exite esse exército nos registros...\n Retorne ao menu e crie um novo.\n";
        return;
    }

    cout << "\nDigite o nome do segundo exercito: ";
    cin >> nome;

    flag = 0;

    for (size_t i = 0; i<tropas.size(); i++)
    {
        if (tropas[i]->getNome() == nome)
        {
            tropa2 = tropas[i];
            flag = 1;
            break;
        } 
    }

    if (flag == 0)
    {
        cout << "\nNão exite esse exército nos registros...\n Retorne ao menu e crie um novo.\n";
        return;
    }

    for (int j = 0; j< UNIDADES; j++)
    {
        tropa1->getUnidades()[j]->setAtributos();
        tropa2->getUnidades()[j]->setAtributos();
    }

    cout << "\nOtimo! Agora digite uma data: ";
    cin >> d >> m >> a;

    Date data(d, m, a);

    Batalha battle (data, tropa1, tropa2);

    cout << "\nOBSERVAÇÕES: \n1 - As estatísticas da batalha não contarão para a tabela de posições da campanha.\n2 - Ao realizar o ataque de um dos exércitos, o exército atacado NÃO poderá revidar, ou seja, mesmo que o poder de ataque de uma das unidades do exército atacado seja maior que o da unidade do exército ofensivo, o resultado do exército atacado NÃO será incrementado...";

    cout << "\n\nDigite 1 para realizar um ataque do primeiro exército, 2 para realizar um ataque do segundo exército, e 3 para realizar um ataque de ambos os exércitos: ";
    cin >> ataque;

    switch (ataque)
    {
        case 1:
            battle.ataqueExercitoA();
            break;
        
        case 2:
            battle.ataqueExercitoB();
            break;

        case 3:
            battle.ataqueExercitoA();
            battle.ataqueExercitoB();
            break;
        
        default: 
            cout << "\nOpção Inválida!\n";
    }

    cout << "\nBatalha do dia " << data.formatarData() << "\n";

    cout << "\nResultado: " <<  "\n" << battle.getResultado() << "\n";
}

void criaExercito(vector<Exercito*>& tropas)
{
    string nome;
    int derrotas, vitorias, empates;
    try{
        cout << "\nDigite o nome do exercito: ";
        if(!(cin >> nome)) throw runtime_error("");

        cout << "\nDigite o numero de derrotas (caso nao queira especificar, digite 0): ";
        if(!(cin >> derrotas)) throw runtime_error("");

        cout << "\nDigite o numero de vitorias (caso nao queira especificar, digite 0): ";
        if(!(cin >> vitorias)) throw runtime_error("");

        cout << "\nDigite o numero de empates (caso nao queira especificar, digite 0): ";
        if(!(cin >> empates)) throw runtime_error("");

        Exercito* tropa = new Exercito(nome, vitorias, derrotas, empates);

        for (int j = 0; j<AERO; j++)
        {
            Aeronave *a1 = new Aeronave();
            a1->setAtributos();
            tropa->adicionarUnidade(a1);
        }

        for (int j = 0; j<INFAN; j++)
        {
            Infantaria *i1 = new Infantaria();
            i1->setAtributos();
            tropa->adicionarUnidade(i1);
        }

        for (int j = 0; j<VEIC; j++)
        {
            Veiculo *v1 = new Veiculo();
            v1->setAtributos();
            tropa->adicionarUnidade(v1);
        }

        tropas.push_back(tropa);
    }
    catch(bad_alloc& e){
        cout << "\nErro de alocacao\n";
    }

    catch(...){
        cout << "Tipo incorreto na leitura ";
    }

    cout << "\nOtimo! Exercito criado com sucesso!\n";
}

void imprimeExercito(vector<Exercito*> tropas)
{
    string resposta;

    cout << "\nDigite o nome do exercito que deseja imprimir, ou TODOS para imprimir as informações de todos os exércitos existentes: ";
    cin >> resposta;

    if (resposta == "TODOS")
    {
        for (size_t i = 0; i<tropas.size(); i++)
        {
            cout << "\n============================================";
            cout << "\nNome do Exército: " << tropas[i]->getNome();
            cout << "\n\nQuantidade de Vitórias: " << tropas[i]->getVitoria();
            cout << "\nQuantidade de Derrotas: " << tropas[i]->getDerrota();
            cout << "\nQuantidade de Empates: " << tropas[i]->getEmpate();
            cout << "\n\nUnidades: ";
            tropas[i]->imprimeUnidades();
            cout << "\n============================================";
            cout << "\n";
        }

        return;
    }

    for (size_t i = 0; i<tropas.size(); i++)
    {
        if (tropas[i]->getNome() == resposta)
        {
            cout << "\n============================================";
            cout << "\nNome do Exército: " << tropas[i]->getNome();
            cout << "\n\nQuantidade de Vitórias: " << tropas[i]->getVitoria();
            cout << "\nQuantidade de Derrotas: " << tropas[i]->getDerrota();
            cout << "\nQuantidade de Empates: " << tropas[i]->getEmpate();
            cout << "\n\nUnidades: ";
            tropas[i]->imprimeUnidades();
            cout << "\n============================================";
            cout << "\n";

            return;
        }
        
    }

    cout << "\nNão foi possível encontrar esse exército!\n";
}

void competicao (vector <Exercito*> tropas, Campanha *camp)
{
    Date data;

    if (tropas.size() < 2)
    {
        cout << "\nNúmero de exércitos insuficientes!\n";
        return;
    }
    //criar todas as batalhas de todos os exercitos contra todos
    for (size_t i = 0; i<tropas.size(); i++)
    {
        for (size_t j = i+1; j<tropas.size(); j++)
        {
            Batalha* battle = new Batalha(data, tropas[i],tropas[j]);
            camp->setBatalha(battle);
        }
    }

    camp->simularBatalhas();
    camp->gerarTabelaDePosicoes(tropas);
    camp->mostrarUnidadeMaisDestrutiva(tropas);
}
