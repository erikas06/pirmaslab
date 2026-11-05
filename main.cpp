#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "studentas.h"
#include "failai.h"

using namespace std;

int main() {

    vector<Studentas> studentai;

    double nuskaitymoLaikas = 0;
    double grupavimoLaikas = 0;
    double irasymoLaikas = 0;

    cout << "Pasirinkite veiksma:" << endl;
    cout << "1 - Ivesti studentus" << endl;
    cout << "2 - Nuskaityti studentus is failo" << endl;
    cout << "3 - Generuoti studentu faila" << endl;
    cout << "4 - Padalinti studentus i grupes" << endl;
    cout << "0 - Baigti programa" << endl;

    int veiksmas;
    cin >> veiksmas;

    while (veiksmas != 0) {

        if (veiksmas == 1) {

            bool testiIvedima = true;

            while (testiIvedima) {

                Studentas studentas;

                cout << "Iveskite studento varda: ";
                cin >> studentas.vardas;

                cout << "Iveskite studento pavarde: ";
                cin >> studentas.pavarde;

                cout << "Pasirinkite pazymiu ivedimo buda:" << endl;
                cout << "1 - Ivesti pazymius paciam" << endl;
                cout << "2 - Sugeneruoti atsitiktinius pazymius" << endl;

                int pazymiuIvedimas;

                while (true) {

                    cin >> pazymiuIvedimas;

                    if (pazymiuIvedimas == 1 ||
                        pazymiuIvedimas == 2) {
                        break;
                    }

                    cout << "Neteisingas pasirinkimas. "
                         << "Iveskite 1 arba 2: ";
                }

                if (pazymiuIvedimas == 1) {

                    cout << "Iveskite studento namu darbu pazymius "
                         << "(norint pabaigti - iveskite 0):"
                         << endl;

                    int pazymys;

                    while (true) {

                        cin >> pazymys;

                        if (pazymys == 0 &&
                            !studentas.nd.empty()) {
                            break;
                        }

                        if (pazymys >= 1 &&
                            pazymys <= 10) {

                            studentas.nd.push_back(pazymys);
                        }
                        else {

                            cout << "Pazymys turi buti nuo 1 iki 10."
                                 << endl;
                        }
                    }

                    cout << "Iveskite studento egzamino pazymi: ";

                    while (true) {

                        cin >> studentas.egzaminas;

                        if (studentas.egzaminas >= 1 &&
                            studentas.egzaminas <= 10) {
                            break;
                        }

                        cout << "Pazymys turi buti nuo 1 iki 10."
                             << endl;
                    }
                }

                else {

                    int kiekis;

                    while (true) {

                        cout << "Kiek namu darbu pazymiu "
                             << "sugeneruoti? ";

                        cin >> kiekis;

                        if (kiekis > 0) {
                            break;
                        }

                        cout << "Turi buti sugeneruotas bent "
                             << "vienas pazymys."
                             << endl;
                    }

                    generuotiPazymius(studentas, kiekis);

                    cout << "Sugeneruoti namu darbu pazymiai: ";

                    for (int pazymys : studentas.nd) {
                        cout << pazymys << " ";
                    }

                    cout << endl;

                    cout << "Sugeneruotas egzamino pazymys: "
                         << studentas.egzaminas
                         << endl;
                }

                studentai.push_back(studentas);

                char pasirinkimas;

                cout << "Ar norite ivesti dar viena studenta? "
                     << "(t/n): ";

                cin >> pasirinkimas;

                if (pasirinkimas == 'n') {
                    testiIvedima = false;
                }
            }
        }

        else if (veiksmas == 2) {

            string failoPavadinimas;

            cout << "Iveskite failo pavadinima: ";
            cin >> failoPavadinimas;

            studentai.clear();

            nuskaitymoLaikas =
                skaitytiIsFailo(
                    studentai,
                    failoPavadinimas
                );
        }

        else if (veiksmas == 3) {

            string failoPavadinimas;
            int studentuKiekis;

            cout << "Iveskite failo pavadinima: ";
            cin >> failoPavadinimas;

            cout << "Iveskite studentu skaiciu: ";
            cin >> studentuKiekis;

            if (studentuKiekis > 0) {

                generuotiFaila(
                    failoPavadinimas,
                    studentuKiekis
                );

                studentai.clear();
                nuskaitymoLaikas =
                    skaitytiIsFailo(studentai, failoPavadinimas);

                if (studentai.empty()) {
                    cout << "Nepavyko nuskaityti sugeneruoto failo."
                         << endl;
                }
            }
            else {

                cout << "Studentu skaicius turi buti "
                     << "didesnis uz 0."
                     << endl;
            }
        }

        else if (veiksmas == 4) {

            vector<Studentas> maziaukieti;
            vector<Studentas> kieti;

            auto pradziaGrupavimas =
                chrono::high_resolution_clock::now();

            padalintiStudentus(
                studentai,
                maziaukieti,
                kieti
            );

            auto pabaigaGrupavimas =
                chrono::high_resolution_clock::now();

            chrono::duration<double> trukmeGrupavimas =
                pabaigaGrupavimas - pradziaGrupavimas;

            grupavimoLaikas =
                trukmeGrupavimas.count();

            cout << "Studentu padalijimas baigtas."
                 << endl;

            cout << "Maziaukieti: "
                 << maziaukieti.size()
                 << endl;

            cout << "Kieti: "
                 << kieti.size()
                 << endl;

            cout << "Studentu grupavimas truko "
                 << fixed << setprecision(6)
                 << grupavimoLaikas
                 << " s."
                 << endl;

            cout << endl;

            cout << "Pasirinkite rusiavimo buda:"
                 << endl;

            cout << "1 - Pagal pavarde" << endl;
            cout << "2 - Pagal varda" << endl;
            cout << "3 - Pagal galutini bala" << endl;

            int rusiavimoPasirinkimas;

            while (true) {

                cin >> rusiavimoPasirinkimas;

                if (rusiavimoPasirinkimas >= 1 &&
                    rusiavimoPasirinkimas <= 3) {
                    break;
                }

                cout << "Neteisingas pasirinkimas. "
                     << "Iveskite 1, 2 arba 3: ";
            }

            auto pradziaIrasymas =
                chrono::high_resolution_clock::now();

            rasytiRezultatus(
                maziaukieti,
                "maziaukieti.txt",
                rusiavimoPasirinkimas
            );

            rasytiRezultatus(
                kieti,
                "kieti.txt",
                rusiavimoPasirinkimas
            );

            auto pabaigaIrasymas =
                chrono::high_resolution_clock::now();

            chrono::duration<double> trukmeIrasymas =
                pabaigaIrasymas - pradziaIrasymas;

            irasymoLaikas =
                trukmeIrasymas.count();

            cout << "Rezultatu failu irasymas truko "
                 << fixed << setprecision(6)
                 << irasymoLaikas
                 << " s."
                 << endl;
        }

        else {

            cout << "Neteisingas pasirinkimas."
                 << endl;
        }

        cout << endl;

        cout << "Pasirinkite veiksma:" << endl;
        cout << "1 - Ivesti studentus" << endl;
        cout << "2 - Nuskaityti studentus is failo" << endl;
        cout << "3 - Generuoti studentu faila" << endl;
        cout << "4 - Padalinti studentus i grupes" << endl;
        cout << "0 - Baigti programa" << endl;

        cin >> veiksmas;
    }

    if (studentai.empty()) {
        return 0;
    }

    cout << endl;

    cout << "TESTAVIMO LAIKAI"
         << endl;

    cout << "Failo nuskaitymas:     "
         << fixed << setprecision(6)
         << nuskaitymoLaikas
         << " s"
         << endl;

    cout << "Studentu grupavimas:   "
         << grupavimoLaikas
         << " s"
         << endl;

    cout << "Rezultatu irasymas:    "
         << irasymoLaikas
         << " s"
         << endl;

    cout << endl;

    cout << "Pasirinkite galutinio balo "
         << "skaiciavimo buda:"
         << endl;

    cout << "1 - Vidurkis" << endl;
    cout << "2 - Mediana" << endl;

    int pasirinkimas;

    while (true) {

        cin >> pasirinkimas;

        if (pasirinkimas == 1 ||
            pasirinkimas == 2) {
            break;
        }

        cout << "Neteisingas pasirinkimas. "
             << "Iveskite 1 arba 2: ";
    }

    vector<pair<Studentas, double>> galutiniaiRezultatai;

    for (const Studentas& studentas : studentai) {

        if (studentas.nd.empty()) {
            continue;
        }

        double ndVidurkis = 0;

        for (int pazymys : studentas.nd) {
            ndVidurkis += pazymys;
        }

        ndVidurkis /= studentas.nd.size();

        vector<int> surikiuotiPazymiai = studentas.nd;

        sort(
            surikiuotiPazymiai.begin(),
            surikiuotiPazymiai.end()
        );

        double ndMediana;

        if (surikiuotiPazymiai.size() % 2 == 1) {

            ndMediana =
                surikiuotiPazymiai[
                    surikiuotiPazymiai.size() / 2
                ];
        }
        else {

            ndMediana =
                (
                    surikiuotiPazymiai[
                        surikiuotiPazymiai.size() / 2 - 1
                    ]
                    +
                    surikiuotiPazymiai[
                        surikiuotiPazymiai.size() / 2
                    ]
                ) / 2.0;
        }

        double galutinis;

        if (pasirinkimas == 1) {
            galutinis =
                0.4 * ndVidurkis +
                0.6 * studentas.egzaminas;
        }
        else {
            galutinis =
                0.4 * ndMediana +
                0.6 * studentas.egzaminas;
        }

        galutiniaiRezultatai.push_back({studentas, galutinis});
    }

    
    sort(
        galutiniaiRezultatai.begin(),
        galutiniaiRezultatai.end(),
        [](const pair<Studentas, double>& a,
           const pair<Studentas, double>& b) {
            return a.second > b.second;
        }
    );

    ofstream rezultatuFailas("galutiniai_rezultatai.txt");

    if (!rezultatuFailas) {
        cout << "Nepavyko sukurti galutiniai_rezultatai.txt failo."
             << endl;
        return 1;
    }

    rezultatuFailas << left
                    << setw(20) << "Vardas"
                    << setw(20) << "Pavarde"
                    << setw(15) << "Galutinis"
                    << endl;

    rezultatuFailas << string(55, '-') << endl;
    rezultatuFailas << fixed << setprecision(2);

    for (const auto& rezultatas : galutiniaiRezultatai) {
        rezultatuFailas << left
                        << setw(20) << rezultatas.first.vardas
                        << setw(20) << rezultatas.first.pavarde
                        << setw(15) << rezultatas.second
                        << endl;
    }

    rezultatuFailas.close();

    cout << "Rezultatai surikiuoti pagal galutini bala ir irasyti i "
         << "galutiniai_rezultatai.txt"
         << endl;

    return 0;
}