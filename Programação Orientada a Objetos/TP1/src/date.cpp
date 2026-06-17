#include "include/date.h"
#include <string.h>
#include <iomanip>
#include <sstream>

Date::Date(int d, int m, int a) : dia(d), mes(m), ano(a) {}

std::string Date::formatarData() const {
    auto out = std::ostringstream();  
    if (dia < 10) out << "0";
    out << dia << "/";
    if (mes < 10) out << "0";
    out << mes << "/";
    out << ano;
    return out.str();
}

