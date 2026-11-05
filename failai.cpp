#include "failai.h"
#include <fstream>
#include <sstream>
#include <random>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>

double skaitytiIsFailo(vector<Studentas>& studentai,
                       string failoPavadinimas) {

    ifstream failas(failoPavadinimas);

    if (!failas.is_open()) {
        cout << "Nepavyko atidaryti failo." << endl;
        return 0;
    }

    auto pradzia =
        chrono::high_resolution_clock::now();

    string eilute;

    getline(failas, eilute);

    while (getline(failas, eilute)) {

        stringstream ss(eilute);
        Studentas studentas;

        ss >> studentas.vardas
           >> studentas.pavarde;

        vector<int> pazymiai;
        int pazymys;

        while (ss >> pazymys) {
            pazymiai.push_back(pazymys);
        }

        if (!pazymiai.empty()) {

            studentas.egzaminas =
                pazymiai.back();

            pazymiai.pop_back();

            studentas.nd =
                pazymiai;

            studentai.push_back(studentas);
        }
    }

    auto pabaiga =
        chrono::high_resolution_clock::now();

    chrono::duration<double> trukme =
        pabaiga - pradzia;

    failas.close();

    cout << "Studentai nuskaityti is failo."
         << endl;

    cout << "Failo nuskaitymas truko "
         << fixed << setprecision(6)
         << trukme.count()
         << " s."
         << endl;

    return trukme.count();
}


void generuotiFaila(string failoPavadinimas,
                    int studentuKiekis) {

    ofstream failas(failoPavadinimas);

    if (!failas.is_open()) {
        cout << "Nepavyko sukurti failo."
             << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10);

    auto pradzia =
        chrono::high_resolution_clock::now();

    failas << "Vardas Pavarde ";

    for (int i = 1; i <= 15; i++) {
        failas << "ND" << i << " ";
    }

    failas << "Egz." << endl;

    for (int i = 1; i <= studentuKiekis; i++) {

        failas << "Vardas" << i << " "
               << "Pavarde" << i << " ";

        for (int j = 0; j < 15; j++) {
            failas << dist(gen) << " ";
        }

        failas << dist(gen) << endl;
    }

    failas.close();

    auto pabaiga =
        chrono::high_resolution_clock::now();

    chrono::duration<double> trukme =
        pabaiga - pradzia;

    cout << fixed << setprecision(3);

    cout << "Failas "
         << failoPavadinimas
         << " sugeneruotas per "
         << trukme.count()
         << " s."
         << endl;
}


void padalintiStudentus(
    const vector<Studentas>& studentai,
    vector<Studentas>& maziaukieti,
    vector<Studentas>& kieti) {

    for (const Studentas& studentas : studentai) {

        double ndVidurkis = 0;

        for (int pazymys : studentas.nd) {
            ndVidurkis += pazymys;
        }

        ndVidurkis /=
            studentas.nd.size();

        double galutinis =
            0.4 * ndVidurkis +
            0.6 * studentas.egzaminas;

        if (galutinis < 5.0) {
            maziaukieti.push_back(studentas);
        }
        else {
            kieti.push_back(studentas);
        }
    }
}


void rasytiRezultatus(
    const vector<Studentas>& studentai,
    string failoPavadinimas,
    int rusiavimoPasirinkimas) {

    vector<Studentas> surikiuoti =
        studentai;

    sort(
        surikiuoti.begin(),
        surikiuoti.end(),
        [rusiavimoPasirinkimas](
            const Studentas& a,
            const Studentas& b) {

            if (rusiavimoPasirinkimas == 1) {
                return a.pavarde < b.pavarde;
            }

            if (rusiavimoPasirinkimas == 2) {
                return a.vardas < b.vardas;
            }

            double galutinisA = 0;
            double galutinisB = 0;

            for (int pazymys : a.nd) {
                galutinisA += pazymys;
            }

            for (int pazymys : b.nd) {
                galutinisB += pazymys;
            }

            galutinisA =
                0.4 *
                (galutinisA / a.nd.size()) +
                0.6 * a.egzaminas;

            galutinisB =
                0.4 *
                (galutinisB / b.nd.size()) +
                0.6 * b.egzaminas;

            return galutinisA < galutinisB;
        }
    );

    ofstream failas(failoPavadinimas);

    if (!failas.is_open()) {
        cout << "Nepavyko sukurti rezultatu failo."
             << endl;
        return;
    }

    failas << left
           << setw(20) << "Vardas"
           << setw(20) << "Pavarde"
           << setw(15) << "Galutinis"
           << endl;

    failas << string(55, '-')
           << endl;

    failas << fixed
           << setprecision(2);

    for (const Studentas& studentas :
         surikiuoti) {

        double ndVidurkis = 0;

        for (int pazymys :
             studentas.nd) {

            ndVidurkis += pazymys;
        }

        ndVidurkis /=
            studentas.nd.size();

        double galutinis =
            0.4 * ndVidurkis +
            0.6 * studentas.egzaminas;

        failas << left
               << setw(20)
               << studentas.vardas
               << setw(20)
               << studentas.pavarde
               << setw(15)
               << galutinis
               << endl;
    }

    failas.close();

    cout << "Rezultatai irasyti i "
         << failoPavadinimas
         << endl;
}