#include <iostream>
#include <fstream>
#include "Structures.h"

using namespace std;

bool vnesiVozilo(Vehicle v);
void validate(Vehicle v);
void promptString(string prompt, string& _str);
void promptInteger(string prompt, int& _int);

int main()
{
    cout << "Kreiranje zapis za vozilo..." << endl;

    while (true)
    {
        Vehicle v;

        promptString("Vnesi ime", v.ime);
        promptString("Vnesi prezime", v.prezime);
        promptString("Vnesi registarski broj (SK-001-AA)", v.regBroj);
        promptString("Vnesi tip na osiguruvanje (fkasko / ffransiza)", v.osig);
        promptInteger("Vnesi kubikaza na motorot (800 - 4000)", v.kubikaza);
        promptInteger("Vnesi data na prizvodstvo (GGMMDD)", v.dataNaProiz);

        if (!vnesiVozilo(v))
        {
            return 1;
        }
        else
        {
            cout << "Voziloto e zacuvano vo sistemot!" << endl;
        }

        char proceed;

        do {
            cout << "Vnesi 'y' za da prodolzis da vnesuvas vozila, vnesi 'n' za prekin: " << endl;
            cin >> proceed;
        } while (proceed != 'y' && proceed != 'n');


        if (proceed == 'n')
        {
            break;
        }
    }

    cout << "Gotovo!";

    return 0;
}

bool vnesiVozilo(Vehicle v)
{
    ofstream outputFile;

    outputFile.open("vozila.dat", ios::app);

    if (!outputFile.is_open())
    {
        cerr << "Nastana greska pri otvoranje na datotekata." << endl;

        return false;
    }

    outputFile
        << v.ime.substr(0, 15) << '|'
        << v.prezime.substr(0, 15) << '|'
        << v.regBroj.substr(0, 9) << '|'
        << v.osig << '|'
        << v.kubikaza << '|'
        << v.dataNaProiz
        << endl;

    outputFile.close();

    return true;
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