#include <iostream>
#include <fstream>
#include "Structures.h"

#define MAX_VEHICLES 4000

using namespace std;

bool vnesiVozila(Vehicle vozila[], int n, string filename);
void validate(Vehicle v);
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
            << "Ime na sopstvenik: " << vozila[i].ime.substr(0, 15) << endl
            << "Prezime na sopstvenik: " << vozila[i].prezime.substr(0, 15) << endl
            << "Registarski broj: " << vozila[i].regBroj.substr(0, 9) << endl
            << "Nacin na osiguruvanje: " << vozila[i].osig << endl
            << "Kubikaza: " << vozila[i].kubikaza << endl
            << "Data na proizvodstvo: " << vozila[i].dataNaProiz
            << "-----------------------------------------------------------------------" << endl;
    }

    outputFile.close();

    return true;
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