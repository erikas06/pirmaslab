#ifndef STUDENTAS_H
#define STUDENTAS_H

#include <string>
#include <vector>

using namespace std;

struct Studentas {
    string vardas;
    string pavarde;
    vector<int> nd;
    int egzaminas;
};

void generuotiPazymius(Studentas& studentas, int kiekis);

#endif