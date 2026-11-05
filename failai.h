#ifndef FAILAI_H
#define FAILAI_H

#include "studentas.h"
#include <vector>
#include <string>

using namespace std;

double skaitytiIsFailo(
    vector<Studentas>& studentai,
    string failoPavadinimas
);

void generuotiFaila(
    string failoPavadinimas,
    int studentuKiekis
);

void padalintiStudentus(
    const vector<Studentas>& studentai,
    vector<Studentas>& maziaukieti,
    vector<Studentas>& kieti
);

void rasytiRezultatus(
    const vector<Studentas>& studentai,
    string failoPavadinimas,
    int rusiavimoPasirinkimas
);

#endif