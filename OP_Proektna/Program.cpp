#define _CRT_SECURE_NO_WARNINGS
#define MAX_VEHICLES 4000

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

struct Vehicle
{
    string ime;
    string prezime;
    string regBroj;
    string osig;
    int kubikaza;
    int dataNaProiz;
};

bool vnesiVozila(Vehicle vozila[], int n, string filename);
void vcitajVozila(string filename, Vehicle* vozila, int& count);
void validate(Vehicle v);
void prikaziCenaDataSiteVozila();
void prikaziCenaData(Vehicle v, int currentYear, int currentMonth, int currentDay);
void ispecatiDataZaSledenTehnicki(int dataNaProiz, int currentYear, int currentMonth, int currentDay);
float presmetajCena(int kubikaza);
float presmetajCenaOsiguruvanje(string tipNaOsig, int kubikaza);
void sortirajVozila(Vehicle vozila[], int n);
void promptString(string prompt, string& _str);
void promptInteger(string prompt, int& _int);

int main()
{
    Vehicle vozila[MAX_VEHICLES];

    int count = 0;
    while (count < MAX_VEHICLES)
    {
        cout << "Kreiranje zapis za vozilo..." << endl;

        promptString("Vnesi ime", vozila[count].ime);
        promptString("Vnesi prezime", vozila[count].prezime);
        promptString("Vnesi registarski broj (SK-001-AA)", vozila[count].regBroj);
        promptString("Vnesi tip na osiguruvanje (fkasko / ffransiza)", vozila[count].osig);
        promptInteger("Vnesi kubikaza na motorot (800 - 4000)", vozila[count].kubikaza);
        promptInteger("Vnesi data na prizvodstvo (GGMMDD)", vozila[count].dataNaProiz);

        count++;

        char proceed;

        do {
            cout << "Vnesi 'y' za da prodolzis da vnesuvas vozila, vnesi 'n' za prekin: " << endl;
            cin >> proceed;
        } while (proceed != 'y' && proceed != 'n');

        if (proceed == 'n')
        {
            break;
        }

        cin.clear();
        cin.ignore(9999, '\n');
    }

    if (!vnesiVozila(vozila, count, "vozila.dat"))
    {
        return 1;
    }
    else
    {
        cout << "Vozilata se zacuvani vo sistemot!" << endl;
    }

    cout << "Sortiranje na podatocite..." << endl;

    sortirajVozila(vozila, count);

    if (!vnesiVozila(vozila, count, "Sort.dat"))
    {
        return 1;
    }

    cout << "Gotovo!" << endl;

    prikaziCenaDataSiteVozila();

    return 0;
}

bool vnesiVozila(Vehicle vozila[], int n, string filename)
{
    ofstream outputFile;

    outputFile.open(filename);

    if (!outputFile.is_open())
    {
        cerr << "Nastana greska pri otvoranje na datotekata." << endl;

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

void vcitajVozila(string filename, Vehicle* vozila, int& count)
{
    ifstream inputFile;

    inputFile.open(filename);

    if (!inputFile.is_open())
    {
        cerr << "Nastana greska pri otvoranje na datotekata." << endl;

        return;
    }

    count = 0;

    while (!inputFile.eof())
    {
        Vehicle v;

        if (!getline(inputFile, v.ime, '|'))
        {
            break;
        }
        getline(inputFile, v.prezime, '|');
        getline(inputFile, v.regBroj, '|');
        getline(inputFile, v.osig, '|');
        string kubikaza;
        getline(inputFile, kubikaza, '|');
        v.kubikaza = stoi(kubikaza);
        string dataNaProiz;
        getline(inputFile, dataNaProiz, '\n');
        v.dataNaProiz = stoi(dataNaProiz);

        vozila[count++] = v;
    }

    inputFile.close();
}

void prikaziCenaDataSiteVozila()
{
    time_t now_c = time(nullptr);
    tm* parts = localtime(&now_c);

    int year = parts->tm_year + 1900;
    int month = parts->tm_mon + 1;
    int day = parts->tm_mday;

    Vehicle vozila[MAX_VEHICLES];
    int count = 0;
    vcitajVozila("Sort.dat", vozila, count);

    for (int i = 0; i < count; i++)
    {
        prikaziCenaData(vozila[i], year, month, day);

        cout << "Pritisni \"Enter\" za da prodlozis ponatamu..." << endl;
        std::cin.get();
    }
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
    int godProiz = dataNaProiz / 10000;
    int mesProiz = (dataNaProiz - godProiz * 10000) / 100;
    int denProiz = (dataNaProiz - mesProiz * 100) % 100;
    godProiz += 2000;

    int denoviProiz = godProiz * 365 + mesProiz * 30 + denProiz;
    int vkupnoDenovi = currentYear * 365 + currentMonth * 30 + currentDay;

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
            if (vozila[i].ime > vozila[j].ime
                ||
                vozila[i].prezime > vozila[j].prezime
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

void promptString(string prompt, string& _str)
{
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
        cerr << "Nevaliden vlez!" << endl;
    }
}

void promptInteger(string prompt, int& _int)
{
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
        cerr << "Nevaliden vlez!" << endl;
    }
}