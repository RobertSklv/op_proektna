#define _CRT_SECURE_NO_WARNINGS
#define MAX_VEHICLES 4000

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Deklariranje na strukturata za vozila
struct Vehicle
{
    string ime;
    string prezime;
    string regBroj;
    string osig;
    int kubikaza;
    int dataNaProiz;
};

// Deklariranje na site funkcii koi kje se upotrebuvaat vo programata
bool vnesiVozila(Vehicle vozila[], int n, string filename);
void prikaziCenaDataSiteVozila();
void prikaziCenaData(Vehicle v, int currentYear, int currentMonth, int currentDay);
void ispecatiDataZaSledenTehnicki(int dataNaProiz, int currentYear, int currentMonth, int currentDay);
float presmetajCena(int kubikaza);
float presmetajCenaOsiguruvanje(string tipNaOsig, int kubikaza);
void sortirajVozila(Vehicle vozila[], int n);
bool validateNameSurname(string str);
bool validateRegistrationNumber(string reg);
bool validateInsuranceType(string ins);
bool validateCubicCapacity(int cc);
bool validateDateOfManufacture(int date);
void promptName(string& name);
void promptSurname(string& surname);
void promptRegistrationNumber(string& regBroj);
void promptInsuranceType(string& ins);
void promptCubicCapacity(int& cc);
void promptDateOfManufacture(int& cc);
void promptString(string prompt, string& _str);
void promptInteger(string prompt, int& _int);

int main()
{
    // Deklariranje na niza od vozila so maksimalna dolzina: 4000
    Vehicle vehicles[MAX_VEHICLES];

    cout << "=== SISTEM ZA TEHNICKI PREGLEDI ===" << endl;
    cout << "Maksimalen kapacitet: " << MAX_VEHICLES << " vozila" << endl << endl;

    // Ciklus koj se izvrsuva se dodeka ne se nadmine maksimalniot broj na vozila
    int count = 0;
    while (count < MAX_VEHICLES)
    {
        cout << "=== KREIRANJE ZAPIS ZA VOZILO ===" << endl << endl;

        // Baranje informacii od korisnikot za zapis na vozilo vo sistemot
        promptName(vehicles[count].ime);
        promptSurname(vehicles[count].prezime);
        promptRegistrationNumber(vehicles[count].regBroj);
        promptInsuranceType(vehicles[count].osig);
        promptCubicCapacity(vehicles[count].kubikaza);
        promptDateOfManufacture(vehicles[count].dataNaProiz);

        // Otkako informaciite se vneseni bez nikakvi detektirani greski, go inkrementirame brojacot
        count++;

        // Deklarirame promenliva koja kje oznacuva dali ciklusot da se izvrsi uste ednas
        char proceed;

        // Se dodeka vlezot ne e 'y' ili 'n', barame od korisnikot da vnese 'y' ili 'n'
        do {
            cout << "Vnesi 'y' za da prodolzis da vnesuvas vozila, vnesi 'n' za prekin: " << endl << endl;
            cin >> proceed;
        } while (proceed != 'y' && proceed != 'n');

        // Dokolku vlezot e 'n', go prekinuvame ciklusot
        if (proceed == 'n')
        {
            break;
        }

        // Gi resetirame site mozni nastanati greski vo vlezot
        cin.clear();

        // Go pomestuvame markerot na slednata nova linija
        cin.ignore(9999, '\n');
    }

    // Gi vnesuvame vozilata
    if (!vnesiVozila(vehicles, count, "vozila.dat"))
    {
        cout << "Vozilata se zacuvani vo sistemot!" << endl << endl;
    }
    else
    {
        // Dokolku vnesiVozila vrati false, toa znaci deka nastanala nekoja greska i mora da zapreme so programata
        return 1;
    }

    cout << "Sortiranje na podatocite..." << endl << endl;

    sortirajVozila(vehicles, count);

    if (!vnesiVozila(vehicles, count, "Sort.dat"))
    {
        // Dokolku vnesiVozila vrati false, toa znaci deka nastanala nekoja greska i mora da zapreme so programata
        return 1;
    }

    cout << "Gotovo!" << endl << endl;

    prikaziCenaDataSiteVozila();

    return 0;
}

// Funkcija za vnesuvanje na podatocite za vozilata vo datoteka
// Podatocite se oddeleni so '|' karakter
// Sekoe vozilo se naogja vo posebna linija
bool vnesiVozila(Vehicle vozila[], int n, string filename)
{
    ofstream outputFile;

    outputFile.open(filename);

    if (!outputFile.is_open())
    {
        cout << "Greska: Nastana greska pri otvoranje na datotekata!" << endl;

        return false;
    }

    for (int i = 0; i < n; i++)
    {
        outputFile
            << vozila[i].ime.substr(0, 15) << '|'
            << vozila[i].prezime.substr(0, 15) << '|'
            << vozila[i].regBroj.substr(0, 9) << '|'
            << vozila[i].osig << '|'
            << vozila[i].kubikaza << '|'
            << vozila[i].dataNaProiz;

        if (i != n - 1)
        {
            outputFile << endl;
        }
    }

    outputFile.close();

    return true;
}

void prikaziCenaDataSiteVozila()
{
    // Vlecenje na momentalnata data od sistemot
    time_t now_c = time(nullptr);
    tm* parts = localtime(&now_c);

    int year = parts->tm_year + 1900;
    int month = parts->tm_mon + 1;
    int day = parts->tm_mday;

    ifstream inputFile;

    // Otvoranje na Sort.dat datotekata
    inputFile.open("Sort.dat");

    if (!inputFile.is_open())
    {
        cout << "Greska: Nastana greska pri otvoranje na datotekata." << endl;

        return;
    }

    // Ciklusot se izvrsuva se dodeka ne se dostigne krajot na datotekata
    while (!inputFile.eof())
    {
        Vehicle v;

        // Dokolku ovde dobieme false kako povratna vrednost, toa znaci
        // deka ne postoi sledna linija i ciklusot mora da se prekine
        if (!getline(inputFile, v.ime, '|'))
        {
            break;
        }

        // Citanje na ostanatite podatoci za voziloto od datotekata
        getline(inputFile, v.prezime, '|');
        getline(inputFile, v.regBroj, '|');
        getline(inputFile, v.osig, '|');
        string kubikaza;
        getline(inputFile, kubikaza, '|');
        v.kubikaza = stoi(kubikaza);
        string dataNaProiz;
        getline(inputFile, dataNaProiz, '\n');
        v.dataNaProiz = stoi(dataNaProiz);

        // Prikazuvanje na cenata i datata za sledniot tehnicki pregled
        prikaziCenaData(v, year, month, day);
    }

    // Zatvoranje na datotekata
    inputFile.close();
}

void prikaziCenaData(Vehicle v, int currentYear, int currentMonth, int currentDay)
{
    float cost = presmetajCena(v.kubikaza);
    float insCost = presmetajCenaOsiguruvanje(v.osig, v.kubikaza);
    float finalCost = cost + insCost;

    cout << "Prezime: " << v.prezime << endl;
    cout << "Ime: " << v.ime << endl;
    cout << "Registarski broj: " << v.regBroj << endl;
    cout << "Cena: " << finalCost << " den." << endl;
    cout << "Sleden tehnicki pregled na: ";
    ispecatiDataZaSledenTehnicki(v.dataNaProiz, currentYear, currentMonth, currentDay);
    cout << endl << endl;
}

void ispecatiDataZaSledenTehnicki(int dataNaProiz, int currentYear, int currentMonth, int currentDay)
{
    // Presmetuvanje na datata na proizvodsvto od edna integer vrednost
    int godProiz = dataNaProiz / 10000;
    int mesProiz = (dataNaProiz - godProiz * 10000) / 100;
    int denProiz = (dataNaProiz - mesProiz * 100) % 100;
    godProiz += 2000;

    // Datata na proizvodstvo pretvorena vo denovi
    int denoviProiz = godProiz * 365 + mesProiz * 30 + denProiz;

    // Denesnata data pretvorena vo denovi
    int vkupnoDenovi = currentYear * 365 + currentMonth * 30 + currentDay;

    // Presmetuvanje na razlikata vo godini
    int godRazlika = (vkupnoDenovi - denoviProiz) / 365;

    int godiniDoSledenTehnicki = 2;

    if (godRazlika >= 8)
    {
        godiniDoSledenTehnicki = 1;
    }

    cout <<
        setw(2) << setfill('0') << to_string(currentDay) << '.' <<
        setw(2) << setfill('0') << to_string(currentMonth) << '.' <<
        setw(4) << setfill('0') << to_string(currentYear + godiniDoSledenTehnicki);
}

float presmetajCena(int kubikaza)
{
    if (kubikaza >= 800 && kubikaza <= 1200) {
        return 13000;
    }
    else if (kubikaza > 1200 && kubikaza <= 1600) {
        return 15000;
    }
    else if (kubikaza > 1600 && kubikaza <= 2000) {
        return 18000;
    }
    else if (kubikaza > 2000 && kubikaza <= 2500) {
        return 20000;
    }
    else if (kubikaza > 2500) {
        return 22000;
    }

    return 0;
}

float presmetajCenaOsiguruvanje(string tipNaOsig, int kubikaza)
{
    float cena = kubikaza * 61.5 / 3;

    if (tipNaOsig == "fkasko")
    {
        return cena;
    }
    else if (tipNaOsig == "ffransiza")
    {
        return cena * 0.5;
    }

    return 0;
}

void sortirajVozila(Vehicle vozila[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (vozila[i].prezime > vozila[j].prezime
                ||
                vozila[i].ime > vozila[j].ime
                ||
                vozila[i].kubikaza > vozila[j].kubikaza)
            {
                Vehicle temp = vozila[i];
                vozila[i] = vozila[j];
                vozila[j] = temp;
            }
        }
    }
}

bool validateNameSurname(string str)
{
    return !str.empty() && str.length() <= 15;
}

bool validateRegistrationNumber(string reg)
{
    if (reg.length() != 9)
        return false;
    if (reg[2] != '-' || reg[6] != '-')
        return false;

    for (int i = 0; i < 9; i++) {
        if (i == 2 || i == 6)
            continue;
        if (!isalnum(reg[i]))
            return false;
    }

    return true;
}

bool validateInsuranceType(string ins)
{
    return ins == "fkasko" || ins == "ffransiza";
}

bool validateCubicCapacity(int cc)
{
    return cc >= 800 && cc <= 4000;
}

bool validateDateOfManufacture(int date)
{
    if (date < 0 || date > 999999) return false;

    int mesec = (date / 100) % 100;
    int den = date % 100;

    if (mesec < 1 || mesec > 12) return false;
    if (den < 1 || den > 31) return false;

    return true;
}

void promptName(string& name)
{
    promptString("Vnesi ime", name);

    if (!validateNameSurname(name))
    {
        cout << "Greska: Imeto ne smee da bide podolgo od 15 karakteri i ne smee da bide prazno!" << endl;
        promptName(name);
    }
}

void promptSurname(string& surname)
{
    promptString("Vnesi prezime", surname);

    if (!validateNameSurname(surname))
    {
        cout << "Greska: Prezimeto ne smee da bide podolgo od 15 karakteri i ne smee da bide prazno!" << endl;
        promptSurname(surname);
    }
}

void promptRegistrationNumber(string& regBroj)
{
    promptString("Vnesi registarski broj (SK-001-AA)", regBroj);

    if (!validateRegistrationNumber(regBroj))
    {
        cout << "Greska: Nevaliden format na registracija! Koristete format: SK-001-AA" << endl;
        promptRegistrationNumber(regBroj);
    }
}

void promptInsuranceType(string& ins)
{
    promptString("Vnesi tip na osiguruvanje (fkasko / ffransiza)", ins);

    if (!validateInsuranceType(ins))
    {
        cout << "Greska: Vnesete 'fkasko' ili 'ffransiza'!" << endl;
        promptInsuranceType(ins);
    }
}

void promptCubicCapacity(int& cc)
{
    promptInteger("Vnesi kubikaza na motorot (800 - 4000)", cc);

    if (!validateCubicCapacity(cc))
    {
        cout << "Greska: Kubikazata mora da bide pomegju 800 i 4000!" << endl;
        promptCubicCapacity(cc);
    }
}

void promptDateOfManufacture(int& cc)
{
    promptInteger("Vnesi data na prizvodstvo (GGMMDD)", cc);

    if (!validateDateOfManufacture(cc))
    {
        cout << "Greska: Nevalidna data! Koristete format GGMMDD" << endl;
        promptDateOfManufacture(cc);
    }
}

void promptString(string prompt, string& _str)
{
    // Ciklusot se izvrsuva se dodeka ne se vnese validen vlez
    while (true)
    {
        cout << prompt << ": ";
        getline(cin, _str);

        if (!cin.fail())
        {
            break;
        }

        cin.clear();
        cin.ignore(9999, '\n');
        cout << "Greska: Nevaliden vlez!" << endl;
    }
}

void promptInteger(string prompt, int& _int)
{
    // Ciklusot se izvrsuva se dodeka ne se vnese validen vlez
    while (true)
    {
        cout << prompt << ": ";
        cin >> _int;

        if (!cin.fail())
        {
            break;
        }

        cin.clear();
        cin.ignore(9999, '\n');
        cout << "Greska: Nevaliden vlez!" << endl;
    }
}