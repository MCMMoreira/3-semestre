#ifndef DATE_H
#define DATE_H

#include <iostream>

using namespace std;

class Date {

    int dia;
    int mes;
    int ano;

public:
    Date(int = 1, int = 1, int = 1999); 
    string formatarData() const;
};

#endif
