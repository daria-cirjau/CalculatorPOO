#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>
#include <vector>
#include <fstream>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;

class Calculator {
public:
    virtual double operatii(double a, char c, double b) = 0; //mt virtuala pura
};

class Ecuatii : public Calculator {
private:
    char* input;
    const double pi = 3.14159265359;
    double rezultatFinal;

public:
    Ecuatii() :pi(3.14159265359)
    {
        input = nullptr;
        rezultatFinal = 0;
    }

    Ecuatii(char* input) :pi(3.14159265359)
    {
        this->input = new char[strlen(input) + 1];
        for (int i = 0; i < strlen(input); i++)
            this->input[i] = input[i];
        this->input[strlen(input)] = '\0';
    }
    ~Ecuatii()
    {
        input = nullptr;
        rezultatFinal = 0;
    }

    char* getInput()
    {
        char* aux = new char[strlen(input) + 1];
        strcpy(aux, this->input);
        return aux;
    }

    void setInput(char* input) {
        this->input = new char[strlen(input) + 1];
        strcpy(this->input, input);
    }

    double getRezultatFinal() {
        int pp = 0;
        int pr = 0;
        for (int i = 0; i < strlen(input); i++)
            if (input[i] == '[')
            {
                pp = 1; break;
            }
        for (int i = 0; i < strlen(input); i++)
            if (input[i] == '(')
            {
                pr = 1; break;
            }

        if (pp == 1)
        {
            rezultatFinal = ecuatieCuParantezePatrate(input);
        }
        else if (pp == 0 && pr == 1)
        {
            rezultatFinal = ecuatieCuParantezeRotunde(input);
        }
        else if (pp == 0 && pr == 0)
        {
            rezultatFinal = ecuatieFaraParanteze(input);
        }


        return rezultatFinal;
    }


    void setRezultatFinal(double rezultatFinal) {
        this->rezultatFinal = rezultatFinal;
    }

    void filtrare(char*& input)
    {
        char* aux = new char[strlen(input)];
        int k = 0;
        for (int i = 0; i < strlen(input); i++)
            if (input[i] != ' ')
            {
                aux[k] = input[i];
                k++;
            }
        aux[k] = '\0';
        //delete[] input;

        input = nullptr;
        input = new char[k];
        for (int i = 0; i < k; i++)
            input[i] = aux[i];
        input[k] = '\0';

        aux = nullptr;
    }

    double operatii(double a, char c, double b) override
    {
        try {
            double rez;
            if (c == '+')
                rez = a + b;
            if (c == '-')
                rez = a - b;
            if (c == '*')
                rez = a * b;
            if (c == '/')
            {
                if (b == 0)
                {
                    throw 1;
                }
                else
                    rez = a / b;
            }
            if (c == '^')
            {
                rez = pow(a, b);
            }
            if (c == '#')
            {
                rez = pow(a, 1 / b);
            }
            return rez;
        }
        catch (int i)
        {
            if (i = 1)
            {
                cout << "Numitorul nu poate fi 0.";
                exit(1);
            }
        }
    }

    void convertToDouble(char* input, double*& numere, int*& vectorPozitii, char*& vectorOperatori, int& dim)
    {
        filtrare(input);
        int m = strlen(input);
        int k = 0;
        int n = -1;

        for (int i = 0; i < m; i++)
            if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^' ||
                input[i] == '#' || input[i] == '[' || input[i] == ']'
                || input[i] == '(' || input[i] == ')') {
                dim++;
            }

        vectorPozitii = new int[dim + 1];
        vectorPozitii[0] = -1;


        vectorOperatori = new char[dim + 1];
        for (int i = 0; i < strlen(input); i++)
            if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '^' ||
                input[i] == '#' || input[i] == '[' || input[i] == ']'
                || input[i] == '(' || input[i] == ')') {
                k++;
                vectorPozitii[k] = i;
                vectorOperatori[k - 1] = input[i];
            }
        vectorOperatori[dim] = '\0';
        vectorPozitii[dim + 1] = strlen(input);

        numere = new double[dim + 1];
        int nm = 0;
        try {
            for (int i = 0; i < dim + 1; i++)
                numere[i] = 0;
            for (int i = 1; i <= k + 1; i++) {
                nm = 0;
                n++;
                int q = 1;
                int ok = 0;
                for (int j = vectorPozitii[i - 1] + 1; j < vectorPozitii[i]; j++) {
                    if (input[j] == ',' || input[j] == '.') {
                        nm++;
                        ok = 1;
                        j++;
                    }
                    if (nm > 1)
                        throw 1;
                    else {
                        if (ok == 0)
                            numere[n] = numere[n] * 10 + (input[j] - '0');
                        if (ok == 1) {
                            numere[n] += (double)(input[j] - '0') / (10 * q);
                            q *= 10;
                        }
                    }
                }
            }
        }
        catch (int i) {
            cout << "Numar invalid.";
            exit(1);
        }


    }

    double ecuatieFaraParanteze(char* input)
    {
        double* numere;
        int* vectorPozitii;
        char* vectorOperatori;
        int dim = 0;

        convertToDouble(input, numere, vectorPozitii, vectorOperatori, dim);
        double rezultat = 0;


        int k = dim;

        //char vectorPrioritate[4] = { '^', '#', '*', '/' };

        for (int j = 0; j < dim; j++) {
            if (vectorOperatori[j] == '^' || vectorOperatori[j] == '#') {

                rezultat = operatii(numere[j], vectorOperatori[j], numere[j + 1]);

                numere[j] = operatii(numere[j], vectorOperatori[j], numere[j + 1]);
                k--;
                dim--;
                for (int q = j + 1; q < k + 1; q++)
                    numere[q] = numere[q + 1];

                for (int q = j; q < dim; q++)
                    vectorOperatori[q] = vectorOperatori[q + 1];

                j--;
            }



        }
        for (int j = 0; j < dim; j++) {
            if (vectorOperatori[j] == '*' || vectorOperatori[j] == '/') {

                rezultat = operatii(numere[j], vectorOperatori[j], numere[j + 1]);

                numere[j] = operatii(numere[j], vectorOperatori[j], numere[j + 1]);
                k--;
                dim--;
                for (int q = j + 1; q < k + 1; q++)
                    numere[q] = numere[q + 1];

                for (int q = j; q < dim; q++)
                    vectorOperatori[q] = vectorOperatori[q + 1];

                j--;
            }

        }
        for (int j = 0; j < dim; j++)
            if (vectorOperatori[j] == '+' || vectorOperatori[j] == '-')
            {
                {
                    rezultat = (double)operatii(numere[j], vectorOperatori[j], numere[j + 1]);

                    numere[j] = operatii(numere[j], vectorOperatori[j], numere[j + 1]);
                    k--;
                    dim--;
                    for (int q = j + 1; q < k + 1; q++)
                        numere[q] = numere[q + 1];

                    for (int q = j; q < dim; q++)
                        vectorOperatori[q] = vectorOperatori[q + 1];

                    j--;
                }
            }

        if (vectorOperatori[0] == '\0')
            rezultat = numere[0];

        if (numere != nullptr)
        {
            //delete[] numere;
            numere = nullptr;
        }

        if (vectorPozitii != nullptr)
        {
            //delete[] vectorPozitii;
            vectorPozitii = nullptr;
        }

        if (vectorOperatori != nullptr)
        {
            //delete[] vectorOperatori;
            vectorOperatori = nullptr;
        }

        return rezultat;
    }

    double ecuatieCuParantezeRotunde(char* input)
    {
        int k = 0;
        double aux;
        char* ec = new char[strlen(input)];
        double rezultatFinal = 0;

        for (int i = 0; i < strlen(input); i++)
        {

            if (input[i] == '(')
            {
                int pozPd = i;
                int pozPi;
                i++;

                while (input[i] != ')')
                {
                    ec[k] = input[i];
                    i++;
                    k++;
                    pozPi = i;
                }
                ec[k] = '\0';
                aux = ecuatieFaraParanteze(ec);

                double auxChar = aux;


                int nrCifre = 0;
                while (auxChar != 0)
                {
                    nrCifre++;
                    auxChar = (int)auxChar / 10;
                }


                //rezultatul din paranteze copiat intr un char

                char* rezChar = new char[nrCifre];

                int auxx = 0;
                while (aux != 0)
                {
                    auxx = auxx * 10 + (int)aux % 10;
                    aux = (int)aux / 10;
                }

                aux = auxx;

                for (int k = 0; k < nrCifre; k++)
                {
                    rezChar[k] = (int)aux % 10 + '0';
                    aux = (int)aux / 10;
                }
                rezChar[nrCifre] = '\0';



                for (int k = 0; k < nrCifre; k++)

                    for (int k = pozPd; k < pozPd + nrCifre; k++)
                        input[k] = rezChar[k - pozPd];


                int l = strlen(input);
                for (int k = 0; k < l - pozPi; k++)
                {
                    input[pozPd + nrCifre + k] = input[pozPi + k + 1];
                }

                i = pozPd;

                rezChar = nullptr;
            }

            ec = nullptr;

            k = 0;
            ec = new char[strlen(input)];




        }
        rezultatFinal = (double)ecuatieFaraParanteze(input);

        return rezultatFinal;


        //1+2*(3+4)+5*(1+2)


    }

    double ecuatieCuParantezePatrate(char* input)
    {
        int k = 0;
        double aux;
        char* ec = new char[strlen(input)];
        double rezultatFinal = 0;

        //1+2*(3+4)+5*[2*(1+2)]

        for (int i = 0; i < strlen(input); i++)
        {

            if (input[i] == '[')
            {
                int pozPd = i;
                int pozPi;
                i++;

                while (input[i] != ']')
                {
                    ec[k] = input[i];
                    i++;
                    k++;
                    pozPi = i;
                }
                ec[k] = '\0';
                aux = ecuatieCuParantezeRotunde(ec);

                double auxChar = aux;

                int nrCifre = 0;
                while (auxChar != 0)
                {
                    nrCifre++;
                    auxChar = (int)auxChar / 10;
                }


                //rezultatul din paranteze copiat intr un char

                char* rezChar = new char[nrCifre];

                int auxx = 0;
                while (aux != 0)
                {
                    auxx = auxx * 10 + (int)aux % 10;
                    aux = (int)aux / 10;
                }

                aux = auxx;

                for (int k = 0; k < nrCifre; k++)
                {
                    rezChar[k] = (int)aux % 10 + '0';
                    aux = (int)aux / 10;
                }
                rezChar[nrCifre] = '\0';

                for (int k = 0; k < nrCifre; k++)

                    for (int k = pozPd; k < pozPd + nrCifre; k++)
                        input[k] = rezChar[k - pozPd];


                for (int k = pozPd + nrCifre; k < strlen(input) - pozPd; k++)
                {
                    input[k] = input[k + (pozPi - pozPd) - nrCifre + 1];
                }

                //                if (pozPd + nrCifre > strlen(input) - pozPd)
                //                    input[strlen(input)] = '\0';
                //                else{
                input[strlen(input)] = '\0';


                i = pozPd;

                // delete[] rezChar;

                rezChar = nullptr;
            }
            // 1+2*(3+4)+5*(1+2)
            //[40+(5-1)]*2
            //[12*(6+6)]/12
            //delete[] ec;
            ec = nullptr;

            k = 0;
            ec = new char[strlen(input)];

        }
        rezultatFinal = (double)ecuatieCuParantezeRotunde(input);

        return rezultatFinal;

    }

    Ecuatii& operator=(Ecuatii& c)
    {
        if (this != &c)
        {

            if (input != nullptr)
            {
                delete[] input;
                input = nullptr;
            }
            this->input = new char[strlen(c.input) + 1];
            for (int i = 0; i < strlen(c.input); i++)
                this->input[i] = c.input[i];
            input[strlen(c.input)] = '\0';
        }

        return *this;
    }

    Ecuatii(const Ecuatii& c)
    {
        this->input = new char[strlen(c.input) + 1];
        for (int i = 0; i < strlen(c.input); i++)
            this->input[i] = c.input[i];
        input[strlen(c.input)] = '\0';
    }

    Ecuatii operator+(int val)
    {
        Ecuatii copie = *this;
        copie.rezultatFinal += val;
        return copie;
    }

    Ecuatii operator*(int val)
    {
        Ecuatii copie = *this;
        copie.rezultatFinal *= val;
        return copie;
    }



    friend istream& operator>>(istream& in, Ecuatii& c);
    friend ostream& operator<<(ostream& out, Ecuatii c);

    friend ifstream& operator>>(ifstream& in, Ecuatii& c);
    friend ofstream& operator<<(ofstream& out, Ecuatii c);

};
ifstream& operator>>(ifstream& in, Ecuatii& c)
{

    string citire;
    // while(!in.eof())
    getline(in, citire);

    int n = citire.length();
    c.input = new char[n + 1];

    strcpy(c.input, citire.c_str());

    cout << c.getRezultatFinal();

    return in;
}
ofstream& operator<<(ofstream& out, Ecuatii c)
{

    out << setprecision(30) << c.getRezultatFinal();

    return out;
}


istream& operator>>(istream& in, Ecuatii& c)
{
    /// in.get();
    string citire;
    getline(in, citire);
    int n = citire.length();
    c.input = new char[n + 1];

    strcpy(c.input, citire.c_str());

    return in;
}

ostream& operator<<(ostream& out, Ecuatii c)
{
    if (c.getRezultatFinal() == NULL)
        out << "eroare";
    out << setprecision(30) << c.getRezultatFinal();
    return out;
}


class Istoric
{
protected:
    vector<Ecuatii> c;
    int dim;
public:

    Istoric()
    {
        dim = 0;
    }

    Istoric(vector<Ecuatii> c, int dim)
    {
        this->dim = dim;
        for (int i = 0; i < dim; i++)
            this->c.push_back(c[i]);
    }

    ~Istoric()
    {
        dim = 0;
    }

    vector<Ecuatii>& getC() {
        return c;
    }

    void setC(vector<Ecuatii>& c) {
        for (int i = 0; i < c.size(); i++)
            this->c.push_back(c[i]);
    }

    int getDim() {
        return dim;
    }

    void setDim(int dim) {
        this->dim = dim;
    }

    Istoric(const Istoric& is)
    {
        this->c.clear();
        for (int i = 0; i < is.c.size(); i++)
            this->c.push_back(is.c[i]);
    }


    double medie()
    {
        double m = 0;
        for (int i = 0; i < c.size(); i++)
            m += c[i].getRezultatFinal();
        m /= dim;

        return m;
    }

    double max()
    {
        double m = c[0].getRezultatFinal();
        for (int i = 0; i < c.size(); i++)
            if (c[i].getRezultatFinal() > m)
                m = c[i].getRezultatFinal();


        return m;
    }

    virtual void mesajMetodaVirtuala()
    {
        cout << "A fost apelata metoda din clasa de baza(Istoric).";
    }

    virtual void verificaUltimulRezultat()
    {
        if (c[dim - 1].getRezultatFinal() > 0)
            cout << "Ultimul rezultat a fost pozitiv! (clasa baza)";
        else
            cout << "Ultimul rezultat a fost negativ! (clasa baza)";
    }


    Istoric& operator++()
    {
        Ecuatii aux;
        cin >> aux;
        c.push_back(aux);
        dim++;
        return *this;
    }

    Ecuatii operator[](int index)
    {
        if (index >= 0 && index < dim)
        {
            return c[index];
        }
        else return nullptr;
    }


    Istoric& operator=(Istoric& is)
    {
        if (this != &is)
        {
            this->c.clear();
            for (int i = 0; i < is.c.size(); i++)
                this->c.push_back(is.c[i]);
        }
        return *this;
    }


    friend istream& operator>>(istream& in, Istoric& i);
    friend ostream& operator<<(ostream& out, Istoric i);

    friend ifstream& operator>>(ifstream& in, Istoric& i);
    friend ofstream& operator<<(ofstream& out, Istoric i);

};

ifstream& operator>>(ifstream& in, Istoric& i)
{

    i.dim = 0;
    do
    {
        Ecuatii aux;
        i.dim++;
        in >> aux;
        cout << "Rezultat: " << fixed << i.c[i.dim].getRezultatFinal() << endl;

        i.c.push_back(aux);
    } while (strcmp(i.c[i.dim - 1].getInput(), (char*)"exit") != 0);
    return in;
}
ofstream& operator<<(ofstream& out, Istoric i)
{
    for (int j = 0; j < i.c.size() - 1; j++)
    {
        out << "Ecuatia nr: " << j + 1 << " este: " << i.c[j].getInput() << " si are rezultatul: " << i.c[j].getRezultatFinal() << endl;
    }

    return out;
}

ostream& operator<<(ostream& out, Istoric i)
{
    for (int j = 0; j < i.c.size() - 1; j++)
    {
        out << "Ecuatia nr: " << j + 1 << " este: " << i.c[j].getInput() << " si are rezultatul: " << i.c[j].getRezultatFinal() << endl;
    }

    return out;
}

istream& operator>>(istream& in, Istoric& i)
{
    i.dim = 0;
    Ecuatii aux;
    do
    {
        cout << "Introduceti ecuatia nr " << i.dim + 1 << ": ";
        in >> aux;
        if (strcmp(aux.getInput(), (char*)"exit") != 0)
        {
            i.c.push_back(aux);
            cout << "Rezultat: " << fixed << i.c[i.dim].getRezultatFinal() << endl;
            i.dim++;
        }

    } while (strcmp(aux.getInput(), (char*)"exit") != 0);

    return in;
}


class ComparareEcuatii
{
private:
    Ecuatii c;
public:
    ComparareEcuatii()
    {
        c.setInput((char*)"");
    }

    ComparareEcuatii(Ecuatii c)
    {
        this->c = c;
    }


    ComparareEcuatii& operator=(ComparareEcuatii& i)
    {
        if (this != &i)
        {
            this->c = i.c;
        }

        return *this;
    }

    ComparareEcuatii(const ComparareEcuatii& i)
    {
        this->c = c;

    }

    bool notNull()
    {
        return c.getInput() != NULL;
    }

    void greaterValue()
    {
        int dim;
        cout << "Introduceti nr de ecuatii: ";
        cin >> dim;
        cin.get();

        Ecuatii* c = new Ecuatii[dim];

        for (int i = 0; i < dim; i++)
        {
            cin >> c[i];

            if (c[i].getRezultatFinal() > this->c.getRezultatFinal())
                cout << c[i].getInput() << " mai mare cu rezultatul " << c[i].getRezultatFinal() << endl;
            else if (c[i].getRezultatFinal() < this->c.getRezultatFinal())
                cout << this->c.getInput() << " mai mare cu rezultatul " << this->c.getRezultatFinal() << endl;
            else cout << "egale cu rezultatul" << this->c.getRezultatFinal() << endl;
        }

    }

    friend istream& operator>>(istream& in, ComparareEcuatii& i);
    friend ostream& operator<<(ostream& out, ComparareEcuatii i);

    friend bool operator<= (const ComparareEcuatii& c1, const ComparareEcuatii& c2);
    friend bool operator> (const ComparareEcuatii& c1, const ComparareEcuatii& c2);
};

bool operator<= (const ComparareEcuatii& c1, const ComparareEcuatii& c2)
{
    return c1 <= c2;
}
bool operator> (const ComparareEcuatii& c1, const ComparareEcuatii& c2)
{
    return c1 > c2;
}

ostream& operator<<(ostream& out, ComparareEcuatii c)
{
    return out;
}

istream& operator>>(istream& in, ComparareEcuatii& i)
{

    in >> i.c;

    return in;
}

class PrelucreazaRezultat : public Istoric {
private:
    double rezultat;
public:
    PrelucreazaRezultat() :Istoric()
    {
        rezultat = 0;
    }

    ~PrelucreazaRezultat()
    {
        rezultat = 0;
        //cout << "Apel destructor clasa mostentita";
    }

    double getRezultat() {
        return rezultat;
    }

    void setRezultat(double rezultat) {
        //rezultat=c[dim-1].getRezultatFinal();
        this->rezultat = rezultat;
    }

    void schimbareBaza() {
        int i, v[20], x, j, q, aux;
        cout << "Introduceti baza: ";
        cin >> q;
        cout << "Numarul " << rezultat << " in baza " << q << " este: ";
        i = 1;
        x = rezultat;
        while (x >= q)
        {
            v[i] = x % q;
            x = x / q;
            i++;
        }
        v[i] = x;
        for (j = 1; j <= i / 2; j++)
        {
            aux = v[j];
            v[j] = v[i + 1 - j];
            v[i + 1 - j] = aux;
        }
        for (j = 1; j <= i; j++)
            cout << v[j];
    }

    void mesajMetodaVirtuala() override
    {
        cout << "A fost apelata metoda din clasa mostenita(PrelucreazaRezultat).";
    }

    void verificaUltimulRezultat() override
    {
        setRezultat(c[dim - 1].getRezultatFinal());
        if (rezultat > 0)
            cout << "Ultimul rezultat a fost pozitiv! (clasa mostenita)";
        else
            cout << "Ultimul rezultat a fost negativ! (clasa mostenita)";
    }

    friend istream& operator>>(istream& in, PrelucreazaRezultat& i);
    friend ostream& operator<<(ostream& out, PrelucreazaRezultat i);

};

ostream& operator<<(ostream& out, PrelucreazaRezultat i)
{
    return out;
}

istream& operator>>(istream& in, PrelucreazaRezultat& i)
{

    in >> i.rezultat;

    return in;
}

void menu_output()
{
    cout << "1. Introduceti ecuatii pentru a fi rezolvate.  "; cout << "\n";
    cout << "2. Afisati ecuatiile si rezultatele lor in consola. "; cout << "\n";
    cout << "3. Rezolvarea ecuatiilor din fisierul date.in.  "; cout << "\n";
    cout << "4. Afisarea ecuatiilor si rezultatelor in fisierul date.out. "; cout << "\n";
    cout << "5. Calculati valoarea maxima dintre rezultatele ecuatiilor introduse anterior. "; cout << "\n";
    cout << "6. Calculati valoarea medie dintre rezultatele ecuatiilor introduse anterior. "; cout << "\n";
    cout << "7. Salveaza rezultatul curent intr-un fisier binar. " << endl;
    cout << "8. Prelucreaza rezultatul salvat anterior: converteste intr-o baza. " << endl;
    cout << "9. Comparati ecuatii. " << endl;
    cout << "0. Iesire."; cout << "\n";
}
void menu()
{
    int option;
    option = 0;
    Istoric is;
    Ecuatii e;
    do
    {
        menu_output();
        cout << "\nIntroduceti numarul actiunii: ";
        cin >> option;
        cin.get();

        if (option == 1)
        {
            cin >> is;
        }
        if (option == 2)
        {
            cout << is;
        }
        if (option == 3)
        {
            ifstream fin("date.in");
            fin >> e;
            fin.close();
        }
        if (option == 4)
        {
            ofstream fout("date.out");
            fout << e;
            fout.close();
        }
        if (option == 5)
        {
            cout << "Valoarea maxima este: " << is.max();
        }
        if (option == 6)
        {
            cout << "Valoarea medie este: " << is.medie();
        }
        if (option == 7)
        {
            double d = is.operator[](is.getDim() - 1).getRezultatFinal();
            string s = to_string(d);
            fstream g;
            g.open("fb.in", ios::in | ios::binary);
            g.write(reinterpret_cast<char*>(&s), sizeof(double));
            cout << "Rezultatul " << d << " a fost salvat in fisierul binar <fb.in>.";

        }
        if (option == 8)
        {
            PrelucreazaRezultat p;
            p.setRezultat(is.operator[](is.getDim() - 1).getRezultatFinal());
            p.schimbareBaza();
        }
        if (option == 9)
        {
            ComparareEcuatii comp;
            cin >> comp;
            comp.greaterValue();
        }
        if (option == 0)
        {
            cout << "\nEXIT!\n";
        }
        if (option < 0 || option>9)
        {
            cout << "\nSelectie invalida\n";
        }
        cout << "\n";
        system("pause"); ///Pauza - Press any key to continue...
        system("cls");   ///Sterge continutul curent al consolei
    } while (option != 0);
}


int main() {
    //menu();
    cout <<"int*"<< sizeof(int*)<<endl;
    cout << "long*" << sizeof(long*) << endl;
    cout << "float*" << sizeof(float*) << endl;
    cout << "double*" << sizeof(double*) << endl;
    cout << "char*" << sizeof(char*) << endl;
return 0;
}
