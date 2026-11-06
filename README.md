# Studentų rezultatų apdorojimo programa – v0.3

## Projekto aprašymas

Programa skirta studentų namų darbų ir egzamino rezultatams įvesti, saugoti, apskaičiuoti galutinį įvertinimą bei suskirstyti studentus į dvi grupes.

Šioje versijoje papildomai įgyvendintas `std::vector` ir `std::list` konteinerių našumo palyginimas. Programa leidžia išmatuoti pagrindinių operacijų vykdymo laiką, naudojant skirtingo dydžio duomenų rinkinius.

## Pagrindinės funkcijos

- Studentų duomenų įvedimas rankiniu būdu.
- Studentų duomenų nuskaitymas iš failo.
- Atsitiktinių studentų duomenų failų generavimas.
- Galutinio studento įvertinimo apskaičiavimas.
- Studentų suskirstymas į dvi grupes pagal galutinį įvertinimą.
- Rezultatų rūšiavimas ir įrašymas į failus.
- Galimybė pasirinkti `std::vector` arba `std::list`.
- Automatinis abiejų konteinerių našumo palyginimas.

## Naudojamos technologijos

- C++
- C++17
- `std::vector`
- `std::list`
- `std::chrono` vykdymo laikui matuoti

## Kompiuterio techninės charakteristikos

Našumo testai atlikti naudojant šią techninę įrangą:

| Komponentas | Specifikacija |
|---|---|
| Procesorius | Intel Core i5-6440HQ |
| Bazinis procesoriaus dažnis | 2,60 GHz |
| Procesoriaus branduoliai | 4 |
| Loginiai procesoriai | 4 |
| Operatyvioji atmintis | 8 GB DDR4 |
| RAM dažnis | 2133 MHz |
| Diskas | SK hynix SC300 M.2 2280 |
| Disko talpa | 256 GB |
| Disko tipas | SSD |

## Našumo testavimo metodika

Buvo palyginti du konteineriai:

- `std::vector<Studentas>`
- `std::list<Studentas>`

Abiem konteineriams naudoti tie patys sugeneruoti duomenų failai, todėl palyginimas atliktas naudojant vienodus studentų duomenis.

Testavimui naudoti penki skirtingi duomenų kiekiai:

| Studentų skaičius |
|---:|
| 1 000 |
| 10 000 |
| 100 000 |
| 1 000 000 |
| 10 000 000 |

Kiekvienas testas pakartotas 5 kartus. Galutiniams rezultatams apskaičiuoti naudotas penkių pakartojimų aritmetinis vidurkis.

Buvo matuojamas šių operacijų vykdymo laikas:

1. Duomenų nuskaitymas iš failo.
2. Studentų suskirstymas į dvi grupes.
3. Studentų rūšiavimas.
4. Rezultatų įrašymas į failus.

Duomenų failų generavimo laikas matuotas atskirai ir neįtrauktas į konteinerių operacijų palyginimą.

Rūšiavimui naudojami skirtingi metodai:
- `std::vector` atveju – `std::sort()`.
- `std::list` atveju – konteinerio metodas `.sort()`.

Visi laiko rezultatai pateikiami sekundėmis.

## Duomenų failų generavimo laikas

| Studentų skaičius | Generavimo laikas (s) |
|---:|---:|
| 1 000 | 0,004 |
| 10 000 | 0,021 |
| 100 000 | 0,199 |
| 1 000 000 | 2,347 |
| 10 000 000 | 24,469 |

Didėjant studentų skaičiui, proporcingai didėja ir duomenų failo generavimo trukmė.

## Našumo testavimo rezultatai

Toliau pateiktose lentelėse nurodyti penkių pakartojimų vidurkiai.

### 1 000 studentų

| Operacija | vector (s) | list (s) |
|---|---:|---:|
| Nuskaitymas | 0,004381 | 0,004096 |
| Grupavimas | 0,000366 | 0,000417 |
| Rūšiavimas | 0,000585 | 0,000427 |
| Įrašymas | 0,003980 | 0,005476 |

### 10 000 studentų

| Operacija | vector (s) | list (s) |
|---|---:|---:|
| Nuskaitymas | 0,043498 | 0,042607 |
| Grupavimas | 0,003488 | 0,003723 |
| Rūšiavimas | 0,008206 | 0,006337 |
| Įrašymas | 0,026965 | 0,022585 |

### 100 000 studentų

| Operacija | vector (s) | list (s) |
|---|---:|---:|
| Nuskaitymas | 0,377953 | 0,379550 |
| Grupavimas | 0,025736 | 0,034557 |
| Rūšiavimas | 0,102723 | 0,099203 |
| Įrašymas | 0,194281 | 0,231456 |

### 1 000 000 studentų

| Operacija | vector (s) | list (s) |
|---|---:|---:|
| Nuskaitymas | 3,702910 | 3,785010 |
| Grupavimas | 0,283431 | 0,357961 |
| Rūšiavimas | 1,554470 | 1,529919 |
| Įrašymas | 2,109952 | 2,185366 |

### 10 000 000 studentų

| Operacija | vector (s) | list (s) |
|---|---:|---:|
| Nuskaitymas | 38,453718 | 37,987820 |
| Grupavimas | 4,617658 | 5,631821 |
| Rūšiavimas | 22,759193 | 21,124101 |
| Įrašymas | 20,851648 | 22,493875 |

## Rezultatų analizė

### Duomenų nuskaitymas

Abiejų konteinerių duomenų nuskaitymo rezultatai yra labai panašūs. Mažesniuose duomenų rinkiniuose skirtumai yra nedideli. Testuojant 10 milijonų studentų, `list` nuskaitymas buvo šiek tiek greitesnis.

Galima teigti, kad pagal atliktus matavimus nė vienas konteineris neturi ryškaus ir pastovaus pranašumo šioje operacijoje.

### Studentų grupavimas

Grupavimo operacijoje `vector` visuose penkiuose duomenų rinkiniuose buvo greitesnis už `list`.

Didžiausiame teste `vector` grupavimą atliko per 4,618 s, o `list` – per 5,632 s.

Tai rodo, kad šioje programoje `vector` yra efektyvesnis studentų grupavimo operacijoje.

### Rūšiavimas

Rūšiavimo operacijoje `list` visuose penkiuose testuose buvo greitesnis už `vector`.

Didžiausiame duomenų rinkinyje `list` rūšiavimas užtruko 21,124 s, o `vector` – 22,759 s.

Šį rezultatą galima sieti su tuo, kad `std::list` turi savo rūšiavimo metodą `.sort()`, o `std::vector` rūšiuojamas naudojant `std::sort()`.

### Rezultatų įrašymas

Rezultatų įrašymo metu nebuvo vieno pastoviai greitesnio konteinerio.

Mažesniuose duomenų rinkiniuose skirtumai buvo nedideli, o didžiausiame teste `vector` rezultatus įrašė per 20,852 s, kai `list` tam prireikė 22,494 s.

Todėl pagal šiuos matavimus negalima teigti, kad vienas konteineris visais atvejais yra efektyvesnis rezultatų įrašymui.

## Bendros išvados

Atlikus `std::vector` ir `std::list` našumo palyginimą, galima išskirti šias išvadas:

1. Duomenų nuskaitymo metu abiejų konteinerių rezultatai buvo labai panašūs.
2. Studentų grupavimas visais testuotais dydžiais buvo greitesnis naudojant `std::vector`.
3. Rūšiavimas visais testuotais dydžiais buvo greitesnis naudojant `std::list`.
4. Rezultatų įrašymo metu pranašumas priklausė nuo duomenų rinkinio dydžio.
5. Nėra vieno konteinerio, kuris būtų geriausias visoms atliekamoms operacijoms.

Taigi konteinerio pasirinkimas priklauso nuo konkrečios atliekamos operacijos. `std::vector` šioje programoje pasirodė efektyvesnis grupuojant studentus, o `std::list` – rūšiuojant.

## Pastabos apie matavimų tikslumą

Matavimai priklauso nuo kompiuterio techninės įrangos, operacinės sistemos apkrovos ir kitų tuo metu veikiančių procesų.

Ypač dideliuose duomenų rinkiniuose pastebimi atskirų pakartojimų laiko svyravimai. Pavyzdžiui, 10 milijonų studentų grupavimo metu vieno `vector` pakartojimo laikas buvo gerokai didesnis už kitus. Todėl rezultatams vertinti naudojamas penkių pakartojimų vidurkis, tačiau jis nepašalina pavienių nuokrypių įtakos.

Rezultatai apibūdina konkrečią programos realizaciją ir bandymams naudotą kompiuterį, todėl nebūtinai atspindi visų galimų `vector` ir `list` panaudojimo atvejų našumą.

## Projekto failai

- `main.cpp` – pagrindinė programos dalis.
- `studentas.cpp` ir `studentas.h` – studento duomenų struktūra ir jos funkcijos.
- `failai.cpp` ir `failai.h` – duomenų nuskaitymas, generavimas, grupavimas, rūšiavimas, įrašymas ir našumo testavimas.
- `benchmark_rezultatai.csv` – našumo testavimo rezultatai, įskaitant atskirus pakartojimus ir vidurkius.

## Kompiliavimas

Programai kompiliuoti naudojamas C++17 standartą palaikantis `g++` kompiliatorius.

```bash
g++ -std=c++17 -O2 -Wall -Wextra main.cpp studentas.cpp failai.cpp -o programa
```

Programos paleidimas:

```bash
./programa
```

Windows aplinkoje:

```powershell
.\programa.exe
```
