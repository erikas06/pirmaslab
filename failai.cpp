#include "failai.h"
#include <fstream>
#include <sstream>
#include <random>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <type_traits>
#include <limits>
#include <stdexcept>
#include <utility>

using namespace std;
using Laikrodis = chrono::steady_clock;

namespace {
    const int KARTOJIMU_SK = 5;
    const vector<int> STUDENTU_KIEKIAI = {
        1000, 10000, 100000, 1000000, 10000000
    };

    double galutinisVidurkis(const Studentas& s) {
        if (s.nd.empty()) return 0.0;

        double suma = 0.0;
        for (int pazymys : s.nd) suma += pazymys;

        return 0.4 * (suma / s.nd.size()) + 0.6 * s.egzaminas;
    }

    template <typename Konteineris>
    double skaitytiIsFailoBendrai(
        Konteineris& studentai,
        const string& failoPavadinimas,
        bool rodytiLaika) {

        ifstream failas(failoPavadinimas);

        if (!failas.is_open()) {
            cerr << "Nepavyko atidaryti failo: "
                 << failoPavadinimas << endl;
            return -1.0;
        }

        auto pradzia = Laikrodis::now();

        string eilute;
        getline(failas, eilute);

        while (getline(failas, eilute)) {
            stringstream ss(eilute);
            Studentas studentas{};

            ss >> studentas.vardas >> studentas.pavarde;

            int pazymys;
            vector<int> pazymiai;

            while (ss >> pazymys)
                pazymiai.push_back(pazymys);

            if (!pazymiai.empty()) {
                studentas.egzaminas = pazymiai.back();
                pazymiai.pop_back();
                studentas.nd = move(pazymiai);
                studentai.push_back(move(studentas));
            }
        }

        failas.close();
        auto pabaiga = Laikrodis::now();

        double sekundes =
            chrono::duration<double>(pabaiga - pradzia).count();

        if (rodytiLaika) {
            cout << "Studentai nuskaityti is failo. Nuskaitymas truko "
                 << fixed << setprecision(6) << sekundes << " s."
                 << endl;
        }

        return sekundes;
    }

    template <typename Konteineris>
    void padalintiStudentusBendrai(
        const Konteineris& studentai,
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

    template <typename Konteineris>
    void rikiuoti(Konteineris& studentai, int pasirinkimas) {

        auto palyginti = [pasirinkimas](
            const Studentas& a,
            const Studentas& b) {

            if (pasirinkimas == 1)
                return a.pavarde < b.pavarde;

            if (pasirinkimas == 2)
                return a.vardas < b.vardas;

            return galutinisVidurkis(a) < galutinisVidurkis(b);
        };

        if constexpr (is_same_v<Konteineris, vector<Studentas>>) {
            sort(studentai.begin(), studentai.end(), palyginti);
        } else {
            studentai.sort(palyginti);
        }
    }

    template <typename Konteineris>
    double irasytiSurikiuotus(
        const Konteineris& studentai,
        const string& failoPavadinimas) {

        auto pradzia = Laikrodis::now();

        ofstream failas(failoPavadinimas);

        if (!failas.is_open()) {
            cerr << "Nepavyko sukurti rezultatu failo: "
                 << failoPavadinimas << endl;
            return -1.0;
        }

        failas << left << setw(20) << "Vardas"
               << setw(20) << "Pavarde"
               << setw(15) << "Galutinis" << '\n';

        failas << string(55, '-') << '\n';
        failas << fixed << setprecision(2);

        for (const Studentas& s : studentai) {
            failas << left << setw(20) << s.vardas
                   << setw(20) << s.pavarde
                   << setw(15) << galutinisVidurkis(s) << '\n';
        }

        failas.close();

        if (!failas) {
            cerr << "Klaida uzdarant rezultatu faila: "
                 << failoPavadinimas << endl;
            return -1.0;
        }

        auto pabaiga = Laikrodis::now();

        return chrono::duration<double>(pabaiga - pradzia).count();
    }

    template <typename Konteineris>
    void rasytiRezultatusBendrai(
        const Konteineris& studentai,
        const string& failoPavadinimas,
        int pasirinkimas,
        double& rusiavimoLaikas,
        double& irasymoLaikas) {

        Konteineris surikiuoti = studentai;

        auto rusiavimoPradzia = Laikrodis::now();

        rikiuoti(surikiuoti, pasirinkimas);

        auto rusiavimoPabaiga = Laikrodis::now();

        rusiavimoLaikas =
            chrono::duration<double>(
                rusiavimoPabaiga - rusiavimoPradzia).count();

        irasymoLaikas =
            irasytiSurikiuotus(surikiuoti, failoPavadinimas);
    }

    void generuotiFailaSuSekla(
        const string& failoPavadinimas,
        int studentuKiekis,
        unsigned int seklosReiksme) {

        ofstream failas(failoPavadinimas);

        if (!failas.is_open()) {
            throw runtime_error(
                "Nepavyko sukurti failo: " + failoPavadinimas);
        }

        mt19937 gen(seklosReiksme);
        uniform_int_distribution<> dist(1, 10);

        failas << "Vardas Pavarde ";

        for (int i = 1; i <= 15; ++i)
            failas << "ND" << i << " ";

        failas << "Egz.\n";

        for (int i = 1; i <= studentuKiekis; ++i) {
            failas << "Vardas" << i << " Pavarde" << i << " ";

            for (int j = 0; j < 15; ++j)
                failas << dist(gen) << " ";

            failas << dist(gen) << '\n';
        }

        if (!failas)
            throw runtime_error(
                "Klaida rasant faila: " + failoPavadinimas);
    }

    struct MatavimoRezultatai {
        double nuskaitymas = 0.0;
        double grupavimas = 0.0;
        double rusiavimas = 0.0;
        double irasymas = 0.0;
    };

    template <typename Konteineris>
    MatavimoRezultatai atliktiVienaBandyma(
        const string& duomenuFailas,
        const string& prefiksas) {

        MatavimoRezultatai rez;
        Konteineris visiStudentai;

        rez.nuskaitymas = skaitytiIsFailoBendrai(
            visiStudentai, duomenuFailas, false);

        if (rez.nuskaitymas < 0.0)
            throw runtime_error(
                "Nepavyko nuskaityti " + duomenuFailas);

        Konteineris maziaukieti, kieti;

        auto grupavimoPradzia = Laikrodis::now();

        padalintiStudentusBendrai(
            visiStudentai, maziaukieti, kieti);

        auto grupavimoPabaiga = Laikrodis::now();

        rez.grupavimas =
            chrono::duration<double>(
                grupavimoPabaiga - grupavimoPradzia).count();

        // Rusiavimas matuojamas atskirai nuo failu irasymo.
        auto rusiavimoPradzia = Laikrodis::now();

        rikiuoti(maziaukieti, 3);
        rikiuoti(kieti, 3);

        auto rusiavimoPabaiga = Laikrodis::now();

        rez.rusiavimas =
            chrono::duration<double>(
                rusiavimoPabaiga - rusiavimoPradzia).count();

        double irasymas1 = irasytiSurikiuotus(
            maziaukieti,
            prefiksas + "_maziaukieti.txt");

        double irasymas2 = irasytiSurikiuotus(
            kieti,
            prefiksas + "_kieti.txt");

        if (irasymas1 < 0.0 || irasymas2 < 0.0)
            throw runtime_error(
                "Nepavyko irasyti rezultatu failu.");

        rez.irasymas = irasymas1 + irasymas2;

        return rez;
    }

    void irasytiCsvEilute(
        ofstream& csv,
        const string& konteineris,
        int kiekis,
        int kartojimas,
        const MatavimoRezultatai& r) {

        csv << konteineris << ','
            << kiekis << ','
            << kartojimas << ','
            << fixed << setprecision(6)
            << r.nuskaitymas << ','
            << r.grupavimas << ','
            << r.rusiavimas << ','
            << r.irasymas << '\n';
    }
}

double skaitytiIsFailo(
    vector<Studentas>& studentai,
    string failoPavadinimas) {

    return skaitytiIsFailoBendrai(
        studentai, failoPavadinimas, true);
}

double skaitytiIsFailo(
    list<Studentas>& studentai,
    string failoPavadinimas) {

    return skaitytiIsFailoBendrai(
        studentai, failoPavadinimas, true);
}

void generuotiFaila(
    string failoPavadinimas,
    int studentuKiekis) {

    auto pradzia = Laikrodis::now();

    generuotiFailaSuSekla(
        failoPavadinimas,
        studentuKiekis,
        random_device{}());

    auto pabaiga = Laikrodis::now();

    cout << fixed << setprecision(3)
         << "Failas " << failoPavadinimas
         << " sugeneruotas per "
         << chrono::duration<double>(
                pabaiga - pradzia).count()
         << " s." << endl;
}

void padalintiStudentus(
    const vector<Studentas>& studentai,
    vector<Studentas>& maziaukieti,
    vector<Studentas>& kieti) {

    padalintiStudentusBendrai(
        studentai, maziaukieti, kieti);
}

void padalintiStudentus(
    const list<Studentas>& studentai,
    list<Studentas>& maziaukieti,
    list<Studentas>& kieti) {

    padalintiStudentusBendrai(
        studentai, maziaukieti, kieti);
}

void rasytiRezultatus(
    const vector<Studentas>& studentai,
    string failoPavadinimas,
    int pasirinkimas,
    double& rusiavimoLaikas,
    double& irasymoLaikas) {

    rasytiRezultatusBendrai(
        studentai,
        failoPavadinimas,
        pasirinkimas,
        rusiavimoLaikas,
        irasymoLaikas);
}

void rasytiRezultatus(
    const list<Studentas>& studentai,
    string failoPavadinimas,
    int pasirinkimas,
    double& rusiavimoLaikas,
    double& irasymoLaikas) {

    rasytiRezultatusBendrai(
        studentai,
        failoPavadinimas,
        pasirinkimas,
        rusiavimoLaikas,
        irasymoLaikas);
}

void vykdytiNasumoTesta() {

    cout << "\nAutomatinis vector ir list palyginimas.\n"
         << "Kartojimu skaicius kiekvienam dydziui: "
         << KARTOJIMU_SK << "\n"
         << "Bus generuojami penki duomenu failai.\n"
         << "10 000 000 studentu bandymas gali pareikalauti "
         << "daug RAM ir disko vietos.\n\n";

    ofstream csv("benchmark_rezultatai.csv");

    if (!csv.is_open()) {
        cerr << "Nepavyko sukurti benchmark_rezultatai.csv\n";
        return;
    }

    csv << "konteineris,studentu_kiekis,kartojimas,"
           "nuskaitymas_s,grupavimas_s,rusiavimas_s,irasymas_s\n";

    try {
        for (int kiekis : STUDENTU_KIEKIAI) {

            string duomenuFailas =
                "studentai" + to_string(kiekis) + ".txt";

            cout << "Generuojamas "
                 << duomenuFailas << "...\n";

            auto generavimoPradzia = Laikrodis::now();

            // Ta pati sugeneruota byla naudojama abiem konteineriams.
            generuotiFailaSuSekla(
                duomenuFailas,
                kiekis,
                20261106u + kiekis);

            auto generavimoPabaiga = Laikrodis::now();

            double generavimoLaikas =
                chrono::duration<double>(
                    generavimoPabaiga - generavimoPradzia).count();

            cout << "Generavimas: "
                 << fixed << setprecision(3)
                 << generavimoLaikas << " s\n";

            MatavimoRezultatai vectorSuma, listSuma;

            for (int kartojimas = 1;
                 kartojimas <= KARTOJIMU_SK;
                 ++kartojimas) {

                cout << "Dydis " << kiekis
                     << ", kartojimas "
                     << kartojimas << "/"
                     << KARTOJIMU_SK << "\n";

                // Kaitaliojama eiles tvarka, kad vienas konteineris
                // nebutu visada pirmas kiekviename kartojime.

                if (kartojimas % 2 == 1) {

                    MatavimoRezultatai v =
                        atliktiVienaBandyma<vector<Studentas>>(
                            duomenuFailas,
                            "vector_" + to_string(kiekis));

                    irasytiCsvEilute(
                        csv, "vector", kiekis, kartojimas, v);

                    vectorSuma.nuskaitymas += v.nuskaitymas;
                    vectorSuma.grupavimas += v.grupavimas;
                    vectorSuma.rusiavimas += v.rusiavimas;
                    vectorSuma.irasymas += v.irasymas;

                    csv.flush();

                    MatavimoRezultatai l =
                        atliktiVienaBandyma<list<Studentas>>(
                            duomenuFailas,
                            "list_" + to_string(kiekis));

                    irasytiCsvEilute(
                        csv, "list", kiekis, kartojimas, l);

                    listSuma.nuskaitymas += l.nuskaitymas;
                    listSuma.grupavimas += l.grupavimas;
                    listSuma.rusiavimas += l.rusiavimas;
                    listSuma.irasymas += l.irasymas;

                    csv.flush();

                } else {

                    MatavimoRezultatai l =
                        atliktiVienaBandyma<list<Studentas>>(
                            duomenuFailas,
                            "list_" + to_string(kiekis));

                    irasytiCsvEilute(
                        csv, "list", kiekis, kartojimas, l);

                    listSuma.nuskaitymas += l.nuskaitymas;
                    listSuma.grupavimas += l.grupavimas;
                    listSuma.rusiavimas += l.rusiavimas;
                    listSuma.irasymas += l.irasymas;

                    csv.flush();

                    MatavimoRezultatai v =
                        atliktiVienaBandyma<vector<Studentas>>(
                            duomenuFailas,
                            "vector_" + to_string(kiekis));

                    irasytiCsvEilute(
                        csv, "vector", kiekis, kartojimas, v);

                    vectorSuma.nuskaitymas += v.nuskaitymas;
                    vectorSuma.grupavimas += v.grupavimas;
                    vectorSuma.rusiavimas += v.rusiavimas;
                    vectorSuma.irasymas += v.irasymas;

                    csv.flush();
                }
            }

            MatavimoRezultatai vectorVidurkis, listVidurkis;

            vectorVidurkis.nuskaitymas =
                vectorSuma.nuskaitymas / KARTOJIMU_SK;
            vectorVidurkis.grupavimas =
                vectorSuma.grupavimas / KARTOJIMU_SK;
            vectorVidurkis.rusiavimas =
                vectorSuma.rusiavimas / KARTOJIMU_SK;
            vectorVidurkis.irasymas =
                vectorSuma.irasymas / KARTOJIMU_SK;

            listVidurkis.nuskaitymas =
                listSuma.nuskaitymas / KARTOJIMU_SK;
            listVidurkis.grupavimas =
                listSuma.grupavimas / KARTOJIMU_SK;
            listVidurkis.rusiavimas =
                listSuma.rusiavimas / KARTOJIMU_SK;
            listVidurkis.irasymas =
                listSuma.irasymas / KARTOJIMU_SK;

            irasytiCsvEilute(
                csv, "vector_vidurkis", kiekis, 0, vectorVidurkis);

            irasytiCsvEilute(
                csv, "list_vidurkis", kiekis, 0, listVidurkis);

            csv.flush();

            cout << "Vidurkiai irasyti i CSV.\n";
        }

    } catch (const exception& e) {

        cerr << "\nTestavimas sustabdytas: "
             << e.what() << endl;

        cout << "Dalinius rezultatus galima rasti "
             << "benchmark_rezultatai.csv faile.\n";

        return;
    }

    csv.close();

    cout << "\nTestavimas baigtas. "
         << "Rezultatai: benchmark_rezultatai.csv\n";
}
