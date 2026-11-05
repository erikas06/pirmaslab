#include "studentas.h"
#include <random>

void generuotiPazymius(Studentas& studentas, int kiekis) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10);

    for (int i = 0; i < kiekis; i++) {
        studentas.nd.push_back(dist(gen));
    }

    studentas.egzaminas = dist(gen);
}