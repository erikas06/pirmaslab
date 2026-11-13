#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

struct Studentas {
    string vardas;
    string pavarde;
    vector<int> nd;
    int egzaminas;
};

void generuotiPazymius(Studentas& studentas, int kiekis) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10);

    for (int i = 0; i < kiekis; i++) {
        studentas.nd.push_back(dist(gen));
    }

    studentas.egzaminas = dist(gen);
}

void skaitytiIsFailo(vector<Studentas>& studentai, string failoPavadinimas) {
    ifstream failas(failoPavadinimas);

    if (!failas.is_open()) {
        cout << "Nepavyko atidaryti failo." << endl;
        return;
    }

    string eilute;

    getline(failas, eilute);

    while (getline(failas, eilute)) {
        stringstream ss(eilute);
        Studentas studentas;

        ss >> studentas.vardas >> studentas.pavarde;

        vector<int> pazymiai;
        int pazymys;

        while (ss >> pazymys) {
            pazymiai.push_back(pazymys);
        }

        if (!pazymiai.empty()) {
            studentas.egzaminas = pazymiai.back();
            pazymiai.pop_back();
            studentas.nd = pazymiai;

            studentai.push_back(studentas);
        }
    }

    failas.close();

    cout << "Studentai nuskaityti is failo." << endl;
}

int main() {
    vector<Studentas> studentai;

    cout << "Pasirinkite veiksma:" << endl;
    cout << "1 - Ivesti studentus" << endl;
    cout << "2 - Nuskaityti studentus is failo" << endl;
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

                    if (pazymiuIvedimas == 1 || pazymiuIvedimas == 2) {
                        break;
                    }

                    cout << "Neteisingas pasirinkimas. Iveskite 1 arba 2: ";
                }

                if (pazymiuIvedimas == 1) {

                    cout << "Iveskite studento namu darbu pazymius "
                         << "(norint pabaigti - iveskite 0):" << endl;

                    int pazymys;

                    while (true) {
                        cin >> pazymys;

                        if (pazymys == 0 && !studentas.nd.empty()) {
                            break;
                        }

                        if (pazymys >= 1 && pazymys <= 10) {
                            studentas.nd.push_back(pazymys);
                        }
                        else {
                            cout << "Pazymys turi buti nuo 1 iki 10." << endl;
                        }
                    }

                    cout << "Iveskite studento egzamino pazymi: ";

                    while (true) {
                        cin >> studentas.egzaminas;

                        if (studentas.egzaminas >= 1 &&
                            studentas.egzaminas <= 10) {
                            break;
                        }

                        cout << "Pazymys turi buti nuo 1 iki 10." << endl;
                    }

                }
                else {

                    int kiekis;

                    while (true) {
                        cout << "Kiek namu darbu pazymiu sugeneruoti? ";
                        cin >> kiekis;

                        if (kiekis > 0) {
                            break;
                        }

                        cout << "Turi buti sugeneruotas bent vienas pazymys."
                             << endl;
                    }

                    generuotiPazymius(studentas, kiekis);

                    cout << "Sugeneruoti namu darbu pazymiai: ";

                    for (int pazymys : studentas.nd) {
                        cout << pazymys << " ";
                    }

                    cout << endl;

                    cout << "Sugeneruotas egzamino pazymys: "
                         << studentas.egzaminas << endl;
                }

                studentai.push_back(studentas);

                char pasirinkimas;

                cout << "Ar norite ivesti dar viena studenta? (t/n): ";
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

            skaitytiIsFailo(studentai, failoPavadinimas);
        }

        else {

            cout << "Neteisingas pasirinkimas." << endl;
        }

        cout << endl;
        cout << "Pasirinkite veiksma:" << endl;
        cout << "1 - Ivesti studentus" << endl;
        cout << "2 - Nuskaityti studentus is failo" << endl;
        cout << "0 - Baigti programa" << endl;

        cin >> veiksmas;
    }

    if (studentai.empty()) {
        return 0;
    }

    cout << "Pasirinkite galutinio balo skaiciavimo buda:" << endl;
    cout << "1 - Vidurkis" << endl;
    cout << "2 - Mediana" << endl;

    int pasirinkimas;

    while (true) {
        cin >> pasirinkimas;

        if (pasirinkimas == 1 || pasirinkimas == 2) {
            break;
        }

        cout << "Neteisingas pasirinkimas. Iveskite 1 arba 2: ";
    }

    sort(studentai.begin(), studentai.end(),
        [](const Studentas& a, const Studentas& b) {

            if (a.pavarde != b.pavarde) {
                return a.pavarde < b.pavarde;
            }

            return a.vardas < b.vardas;
        });

    cout << endl;

    cout << left
         << setw(20) << "Vardas"
         << setw(20) << "Pavarde"
         << setw(15) << "Galutinis"
         << endl;

    cout << string(55, '-') << endl;

    for (const Studentas& studentas : studentai) {

        double ndVidurkis = 0;

        for (int pazymys : studentas.nd) {
            ndVidurkis += pazymys;
        }

        ndVidurkis /= studentas.nd.size();

        vector<int> surikiuotiPazymiai = studentas.nd;

        sort(surikiuotiPazymiai.begin(), surikiuotiPazymiai.end());

        double ndMediana;

        if (surikiuotiPazymiai.size() % 2 == 1) {

            ndMediana =
                surikiuotiPazymiai[surikiuotiPazymiai.size() / 2];

        }
        else {

            ndMediana =
                (surikiuotiPazymiai[surikiuotiPazymiai.size() / 2 - 1]
                + surikiuotiPazymiai[surikiuotiPazymiai.size() / 2])
                / 2.0;
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

        cout << fixed << setprecision(2);

        cout << left
             << setw(20) << studentas.vardas
             << setw(20) << studentas.pavarde
             << setw(15) << galutinis
             << endl;
    }

    return 0;
}
