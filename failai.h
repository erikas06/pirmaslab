#ifndef FAILAI_H
#define FAILAI_H

#include "studentas.h"
#include <vector>
#include <list>
#include <string>

double skaitytiIsFailo(std::vector<Studentas>& studentai, std::string failoPavadinimas);
double skaitytiIsFailo(std::list<Studentas>& studentai, std::string failoPavadinimas);

void generuotiFaila(std::string failoPavadinimas, int studentuKiekis);

void padalintiStudentus(const std::vector<Studentas>& studentai,
                        std::vector<Studentas>& maziaukieti,
                        std::vector<Studentas>& kieti);

void padalintiStudentus(const std::list<Studentas>& studentai,
                        std::list<Studentas>& maziaukieti,
                        std::list<Studentas>& kieti);

// 1 strategija: studentai kopijuojami i du naujus konteinerius.
void padalintiStudentusStrategija1(
    const std::vector<Studentas>& studentai,
    std::vector<Studentas>& maziaukieti,
    std::vector<Studentas>& kieti);

void padalintiStudentusStrategija1(
    const std::list<Studentas>& studentai,
    std::list<Studentas>& maziaukieti,
    std::list<Studentas>& kieti);

void rasytiRezultatus(const std::vector<Studentas>& studentai,
                      std::string failoPavadinimas,
                      int rusiavimoPasirinkimas,
                      double& rusiavimoLaikas,
                      double& irasymoLaikas);

void rasytiRezultatus(const std::list<Studentas>& studentai,
                      std::string failoPavadinimas,
                      int rusiavimoPasirinkimas,
                      double& rusiavimoLaikas,
                      double& irasymoLaikas);

// Automatinis vector ir list palyginimas su penkiais duomenu kiekiais.
void vykdytiNasumoTesta();

#endif
