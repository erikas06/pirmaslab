#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <chrono>
#include "studentas.h"
#include "failai.h"

using namespace std;
using Laikrodis = chrono::high_resolution_clock;

template <typename Konteineris>
void vykdytiPrograma(Konteineris& studentai) {
    double nuskaitymoLaikas = 0.0;
    double grupavimoLaikas = 0.0;
    double rusiavimoLaikas = 0.0;
    double irasymoLaikas = 0.0;

    int veiksmas;
    do {
        cout << "\nPasirinkite veiksma:\n"
             << "1 - Ivesti studentus\n"
             << "2 - Nuskaityti studentus is failo\n"
             << "3 - Generuoti studentu faila\n"
             << "4 - Padalinti studentus i grupes ir irasyti rezultatus\n"
             << "0 - Baigti programa\n";

        cin >> veiksmas;

        if (veiksmas == 1) {
            bool testiIvedima = true;
            while (testiIvedima) {
                Studentas studentas{};

                cout << "Iveskite studento varda: ";
                cin >> studentas.vardas;
                cout << "Iveskite studento pavarde: ";
                cin >> studentas.pavarde;

                cout << "Pasirinkite pazymiu ivedimo buda:\n"
                     << "1 - Ivesti pazymius paciam\n"
                     << "2 - Sugeneruoti atsitiktinius pazymius\n";

                int budas;
                cin >> budas;

                while (budas != 1 && budas != 2) {
                    cout << "Iveskite 1 arba 2: ";
                    cin >> budas;
                }

                if (budas == 1) {
                    cout << "Iveskite ND pazymius (0 - pabaiga):\n";
                    int pazymys;
                    while (true) {
                        cin >> pazymys;
                        if (pazymys == 0 && !studentas.nd.empty()) break;
                        if (pazymys >= 1 && pazymys <= 10)
                            studentas.nd.push_back(pazymys);
                        else
                            cout << "Pazymys turi buti nuo 1 iki 10. Jei baigete, iveskite 0.\n";
                    }

                    cout << "Iveskite egzamino pazymi: ";
                    do {
                        cin >> studentas.egzaminas;
                        if (studentas.egzaminas < 1 || studentas.egzaminas > 10)
                            cout << "Pazymys turi buti nuo 1 iki 10: ";
                    } while (studentas.egzaminas < 1 || studentas.egzaminas > 10);
                } else {
                    int kiekis;
                    do {
                        cout << "Kiek ND pazymiu sugeneruoti? ";
                        cin >> kiekis;
                    } while (kiekis <= 0);

                    generuotiPazymius(studentas, kiekis);
                }

                studentai.push_back(studentas);
                cout << "Studento objekto adresas atmintyje: "
                     << static_cast<const void*>(&studentai.back()) << endl;

                char pasirinkimas;
                cout << "Ar norite ivesti dar viena studenta? (t/n): ";
                cin >> pasirinkimas;
                testiIvedima = (pasirinkimas != 'n' && pasirinkimas != 'N');
            }
        }
        else if (veiksmas == 2) {
            string failoPavadinimas;
            cout << "Iveskite failo pavadinima: ";
            cin >> failoPavadinimas;
            studentai.clear();
            nuskaitymoLaikas = skaitytiIsFailo(studentai, failoPavadinimas);
        }
        else if (veiksmas == 3) {
            string failoPavadinimas;
            int kiekis;
            cout << "Iveskite failo pavadinima: ";
            cin >> failoPavadinimas;
            cout << "Iveskite studentu skaiciu: ";
            cin >> kiekis;

            if (kiekis > 0) {
                generuotiFaila(failoPavadinimas, kiekis);
                studentai.clear();
                nuskaitymoLaikas = skaitytiIsFailo(studentai, failoPavadinimas);
            } else {
                cout << "Studentu skaicius turi buti didesnis uz 0.\n";
            }
        }
        else if (veiksmas == 4) {
            Konteineris maziaukieti, kieti;

            auto grupavimoPradzia = Laikrodis::now();
            padalintiStudentus(studentai, maziaukieti, kieti);
            auto grupavimoPabaiga = Laikrodis::now();
            grupavimoLaikas = chrono::duration<double>(grupavimoPabaiga - grupavimoPradzia).count();

            cout << "Maziaukieti: " << maziaukieti.size() << endl;
            cout << "Kieti: " << kieti.size() << endl;

            cout << "Pasirinkite rusiavimo buda:\n"
                 << "1 - Pagal pavarde\n"
                 << "2 - Pagal varda\n"
                 << "3 - Pagal galutini bala (vidurki)\n";

            int rusiavimoPasirinkimas;
            do {
                cin >> rusiavimoPasirinkimas;
                if (rusiavimoPasirinkimas < 1 || rusiavimoPasirinkimas > 3)
                    cout << "Iveskite 1, 2 arba 3: ";
            } while (rusiavimoPasirinkimas < 1 || rusiavimoPasirinkimas > 3);

            double r1 = 0.0, i1 = 0.0, r2 = 0.0, i2 = 0.0;
            rasytiRezultatus(maziaukieti, "maziaukieti.txt", rusiavimoPasirinkimas, r1, i1);
            rasytiRezultatus(kieti, "kieti.txt", rusiavimoPasirinkimas, r2, i2);

            rusiavimoLaikas = r1 + r2;
            irasymoLaikas = i1 + i2;

            cout << fixed << setprecision(6)
                 << "Studentu grupavimas truko: " << grupavimoLaikas << " s\n"
                 << "Studentu rusiavimas truko: " << rusiavimoLaikas << " s\n"
                 << "Rezultatu irasymas truko: " << irasymoLaikas << " s\n";
        }
        else if (veiksmas != 0) {
            cout << "Neteisingas pasirinkimas.\n";
        }

    } while (veiksmas != 0);

    cout << "\nTESTAVIMO LAIKAI\n"
         << fixed << setprecision(6)
         << "Failo nuskaitymas: " << nuskaitymoLaikas << " s\n"
         << "Studentu grupavimas: " << grupavimoLaikas << " s\n"
         << "Studentu rusiavimas: " << rusiavimoLaikas << " s\n"
         << "Rezultatu irasymas: " << irasymoLaikas << " s\n";

    if (studentai.empty()) return;

    cout << "\nPasirinkite galutinio balo skaiciavimo buda:\n"
         << "1 - Vidurkis\n"
         << "2 - Mediana\n";

    int pasirinkimas;
    do {
        cin >> pasirinkimas;
        if (pasirinkimas != 1 && pasirinkimas != 2)
            cout << "Iveskite 1 arba 2: ";
    } while (pasirinkimas != 1 && pasirinkimas != 2);

    vector<pair<Studentas, double>> galutiniaiRezultatai;

    for (const Studentas& s : studentai) {
        if (s.nd.empty()) continue;

        double ndVidurkis = 0.0;
        for (int pazymys : s.nd) ndVidurkis += pazymys;
        ndVidurkis /= s.nd.size();

        vector<int> pazymiai = s.nd;
        sort(pazymiai.begin(), pazymiai.end());

        double ndMediana;
        size_t n = pazymiai.size();
        if (n % 2 == 1) ndMediana = pazymiai[n / 2];
        else ndMediana = (pazymiai[n / 2 - 1] + pazymiai[n / 2]) / 2.0;

        double galutinis = pasirinkimas == 1
            ? 0.4 * ndVidurkis + 0.6 * s.egzaminas
            : 0.4 * ndMediana + 0.6 * s.egzaminas;

        galutiniaiRezultatai.push_back({s, galutinis});
    }

    sort(galutiniaiRezultatai.begin(), galutiniaiRezultatai.end(),
         [](const auto& a, const auto& b) { return a.second > b.second; });

    ofstream rezultatuFailas("galutiniai_rezultatai.txt");
    if (!rezultatuFailas) {
        cout << "Nepavyko sukurti galutiniai_rezultatai.txt failo.\n";
        return;
    }

    rezultatuFailas << left << setw(20) << "Vardas"
                    << setw(20) << "Pavarde"
                    << setw(15) << "Galutinis" << endl;
    rezultatuFailas << string(55, '-') << endl;
    rezultatuFailas << fixed << setprecision(2);

    for (const auto& r : galutiniaiRezultatai) {
        rezultatuFailas << left << setw(20) << r.first.vardas
                        << setw(20) << r.first.pavarde
                        << setw(15) << r.second << endl;
    }

    rezultatuFailas.close();
    cout << "Galutiniai rezultatai irasyti i galutiniai_rezultatai.txt\n";
}

int main() {
    cout << "Pasirinkite veiksma:\n"
         << "1 - Dirbti su std::vector\n"
         << "2 - Dirbti su std::list\n"
         << "3 - Automatinis vector ir list nasumo palyginimas\n";

    int pasirinkimas;
    do {
        cin >> pasirinkimas;
        if (pasirinkimas < 1 || pasirinkimas > 3)
            cout << "Iveskite 1, 2 arba 3: ";
    } while (pasirinkimas < 1 || pasirinkimas > 3);

    if (pasirinkimas == 1) {
        vector<Studentas> studentai;
        vykdytiPrograma(studentai);
    } else if (pasirinkimas == 2) {
        list<Studentas> studentai;
        vykdytiPrograma(studentai);
    } else {
        vykdytiNasumoTesta();
    }

    return 0;
}
