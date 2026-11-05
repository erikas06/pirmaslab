#ifndef FAILAI_H
#define FAILAI_H

#include "studentas.h"
#include <vector>
#include <list>
#include <string>

using namespace std;

double skaitytiIsFailo(vector<Studentas>& studentai, string failoPavadinimas);
double skaitytiIsFailo(list<Studentas>& studentai, string failoPavadinimas);

void generuotiFaila(string failoPavadinimas, int studentuKiekis);

void padalintiStudentus(const vector<Studentas>& studentai,
                        vector<Studentas>& maziaukieti,
                        vector<Studentas>& kieti);

void padalintiStudentus(const list<Studentas>& studentai,
                        list<Studentas>& maziaukieti,
                        list<Studentas>& kieti);

void rasytiRezultatus(const vector<Studentas>& studentai,
                      string failoPavadinimas,
                      int rusiavimoPasirinkimas,
                      double& rusiavimoLaikas,
                      double& irasymoLaikas);

void rasytiRezultatus(const list<Studentas>& studentai,
                      string failoPavadinimas,
                      int rusiavimoPasirinkimas,
                      double& rusiavimoLaikas,
                      double& irasymoLaikas);

#endif
