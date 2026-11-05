#include "failai.h"
#include <fstream>
#include <sstream>
#include <random>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <type_traits>

using namespace std;
using Laikrodis = chrono::high_resolution_clock;

template <typename Konteineris>
double skaitytiIsFailoBendrai(Konteineris& studentai, const string& failoPavadinimas) {
    ifstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        cout << "Nepavyko atidaryti failo: " << failoPavadinimas << endl;
        return 0.0;
    }

    auto pradzia = Laikrodis::now();
    string eilute;
    getline(failas, eilute); // praleidziama antraste

    while (getline(failas, eilute)) {
        stringstream ss(eilute);
        Studentas studentas;
        ss >> studentas.vardas >> studentas.pavarde;

        vector<int> pazymiai;
        int pazymys;
        while (ss >> pazymys) pazymiai.push_back(pazymys);

        if (!pazymiai.empty()) {
            studentas.egzaminas = pazymiai.back();
            pazymiai.pop_back();
            studentas.nd = pazymiai;
            studentai.push_back(studentas);
        }
    }

    auto pabaiga = Laikrodis::now();
    failas.close();

    chrono::duration<double> trukme = pabaiga - pradzia;
    cout << "Studentai nuskaityti is failo. Nuskaitymas truko "
         << fixed << setprecision(6) << trukme.count() << " s." << endl;
    return trukme.count();
}

double skaitytiIsFailo(vector<Studentas>& studentai, string failoPavadinimas) {
    return skaitytiIsFailoBendrai(studentai, failoPavadinimas);
}

double skaitytiIsFailo(list<Studentas>& studentai, string failoPavadinimas) {
    return skaitytiIsFailoBendrai(studentai, failoPavadinimas);
}

void generuotiFaila(string failoPavadinimas, int studentuKiekis) {
    ofstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        cout << "Nepavyko sukurti failo." << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10);

    auto pradzia = Laikrodis::now();

    failas << "Vardas Pavarde ";
    for (int i = 1; i <= 15; ++i) failas << "ND" << i << " ";
    failas << "Egz." << endl;

    for (int i = 1; i <= studentuKiekis; ++i) {
        failas << "Vardas" << i << " " << "Pavarde" << i << " ";
        for (int j = 0; j < 15; ++j) failas << dist(gen) << " ";
        failas << dist(gen) << endl;
    }

    failas.close();
    auto pabaiga = Laikrodis::now();
    chrono::duration<double> trukme = pabaiga - pradzia;

    cout << fixed << setprecision(3)
         << "Failas " << failoPavadinimas << " sugeneruotas per "
         << trukme.count() << " s." << endl;
}

double galutinisVidurkis(const Studentas& s) {
    if (s.nd.empty()) return 0.0;
    double suma = 0.0;
    for (int pazymys : s.nd) suma += pazymys;
    return 0.4 * (suma / s.nd.size()) + 0.6 * s.egzaminas;
}

template <typename Konteineris>
void padalintiStudentusBendrai(const Konteineris& studentai,
                               Konteineris& maziaukieti,
                               Konteineris& kieti) {
    maziaukieti.clear();
    kieti.clear();

    for (const Studentas& studentas : studentai) {
        if (studentas.nd.empty()) continue;
        if (galutinisVidurkis(studentas) < 5.0)
            maziaukieti.push_back(studentas);
        else
            kieti.push_back(studentas);
    }
}

void padalintiStudentus(const vector<Studentas>& studentai,
                        vector<Studentas>& maziaukieti,
                        vector<Studentas>& kieti) {
    padalintiStudentusBendrai(studentai, maziaukieti, kieti);
}

void padalintiStudentus(const list<Studentas>& studentai,
                        list<Studentas>& maziaukieti,
                        list<Studentas>& kieti) {
    padalintiStudentusBendrai(studentai, maziaukieti, kieti);
}

template <typename Konteineris>
void rasytiRezultatusBendrai(const Konteineris& studentai,
                             const string& failoPavadinimas,
                             int rusiavimoPasirinkimas,
                             double& rusiavimoLaikas,
                             double& irasymoLaikas) {
    Konteineris surikiuoti = studentai;

    auto rusiavimoPradzia = Laikrodis::now();

    auto palyginti = [rusiavimoPasirinkimas](const Studentas& a, const Studentas& b) {
        if (rusiavimoPasirinkimas == 1) return a.pavarde < b.pavarde;
        if (rusiavimoPasirinkimas == 2) return a.vardas < b.vardas;
        return galutinisVidurkis(a) < galutinisVidurkis(b);
    };

    if constexpr (is_same_v<Konteineris, vector<Studentas>>) {
        sort(surikiuoti.begin(), surikiuoti.end(), palyginti);
    } else {
        surikiuoti.sort(palyginti);
    }

    auto rusiavimoPabaiga = Laikrodis::now();
    rusiavimoLaikas = chrono::duration<double>(rusiavimoPabaiga - rusiavimoPradzia).count();

    auto irasymoPradzia = Laikrodis::now();

    ofstream failas(failoPavadinimas);
    if (!failas.is_open()) {
        cout << "Nepavyko sukurti rezultatu failo: " << failoPavadinimas << endl;
        irasymoLaikas = 0.0;
        return;
    }

    failas << left << setw(20) << "Vardas"
           << setw(20) << "Pavarde" << setw(15) << "Galutinis" << endl;
    failas << string(55, '-') << endl;
    failas << fixed << setprecision(2);

    for (const Studentas& s : surikiuoti) {
        failas << left << setw(20) << s.vardas
               << setw(20) << s.pavarde
               << setw(15) << galutinisVidurkis(s) << endl;
    }

    failas.close();
    auto irasymoPabaiga = Laikrodis::now();
    irasymoLaikas = chrono::duration<double>(irasymoPabaiga - irasymoPradzia).count();
}

void rasytiRezultatus(const vector<Studentas>& studentai, string failoPavadinimas,
                      int rusiavimoPasirinkimas, double& rusiavimoLaikas,
                      double& irasymoLaikas) {
    rasytiRezultatusBendrai(studentai, failoPavadinimas, rusiavimoPasirinkimas,
                            rusiavimoLaikas, irasymoLaikas);
}

void rasytiRezultatus(const list<Studentas>& studentai, string failoPavadinimas,
                      int rusiavimoPasirinkimas, double& rusiavimoLaikas,
                      double& irasymoLaikas) {
    rasytiRezultatusBendrai(studentai, failoPavadinimas, rusiavimoPasirinkimas,
                            rusiavimoLaikas, irasymoLaikas);
}
