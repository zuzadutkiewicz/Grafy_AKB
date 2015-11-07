#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#define MAX_TAB 100
#define MAX_MAC_ORYG 1000

using namespace std;

int macierz[MAX_TAB][MAX_TAB];
int rozmMacierz = 0;

int macierzOryg[MAX_MAC_ORYG][MAX_MAC_ORYG];
int rozmMacierzOryg = 0;


void odczytajZPliku(const char * nazwaPliku);
int czyJednograf();
int sprawdzSprzez(int x, int y);
int czySprzez();
int sprawdzLin(int x, int y);
void przenumTab();
void drukTab(int tryb);
void generujGrafOryg();
void wpiszWierzcholki(int x, int k);
void drukTabOryg(int tryb);
int poprzedniki(int kolumna, int listaPoprz[]);
int nastepniki(int wiersz, int listaNast[]);


int main()
{
    odczytajZPliku("test.csv");
    czyJednograf();
    czySprzez();
    przenumTab();
    drukTab(1);
    generujGrafOryg();
    drukTabOryg(1);

    return 0;
}

void odczytajZPliku(const char * nazwaPliku)
{
    char buflicz[10];
    fstream plik;
    string line;
    plik.open(nazwaPliku, ios::in);
    int wiersz = 0;
    int kolumna = 0;
    while(getline(plik,line))
    {
        const char * buf = line.c_str();
        int i = 0;
        int j = 0;
        //int liczKolumn = 0;
        wiersz++;
        while(true)
        {
            if(buf[i] != '\t' && buf[i] !='\0')
            {
                buflicz[j]=buf[i];
                j++;
            }
            else
            {
                int dana = 0;
                kolumna++;
                buflicz[j] = '\0';
                dana = atoi(buflicz);
                macierz[wiersz-1][kolumna-1] = dana;
                j = 0;
            }
            if(buf[i] == '\0')
                break;
            i++;
        }
        // liczKolumn = kolumna;
        kolumna = 0;
    }
    //liczKolumn == linia;
    rozmMacierz = wiersz;
    plik.close();
    printf("Odczyt z pliku zakonczony powodzeniem.\n");
}

// sprawdz czy macierz zawiera inne liczby niz 0 lub 1
// zwaracane wartosci
//      1 - gdy nie jest
//      0 - gdy jest

int czyJednograf()
{
    for(int z=0; z < rozmMacierz; z++)
        for (int k=0;  k < rozmMacierz; k++)
            if (macierz[z][k] != 1 && macierz[z][k] != 0)
            {
                printf("Podany graf nie jest 1-grafem. Kolumna: %d, wiersz %d\n", k,z);
                return 1;
            }

    printf("Podany graf jest 1-grafem.\n");
    return 0;
}

int sprawdzSprzez(int w1, int w2)
{
    int znaczRoz = 0;
    int znaczIden = 0;

    for (int k =0; k < rozmMacierz; k ++)
        if ((macierz[w1][k] == 0 && macierz[w2][k] == 1))
            znaczRoz = 1;
        else if (macierz[w1][k] == 1 && macierz[w2][k] == 0)
            znaczRoz = 1;
        else if (macierz[w1][k] == 1 && macierz[w2][k] == 1)
            znaczIden = 1;

    if(znaczIden == 1 && znaczRoz == 1)
        return 0;
    else if (znaczIden == 1 && znaczRoz == 0)
        return 1;

    return 2;
}

int czySprzez()
{
    int lx = 0;

    for (int x = 0; x <rozmMacierz; x++)
        for (int y = x + 1; y < rozmMacierz; y++)
        {
            int z = sprawdzSprzez(x,y);
            if (z == 0)
            {
                printf("Graf nie jest grafem sprzezonym. \n");
                return 0;
            }
            else if (z == 1)
            {
                if( sprawdzLin(x, y) == 1 )
                    lx = 1;
            }
        }

    printf("Graf jest grafem sprzezonym. \n");
    if (lx == 0)
        printf("Graf jest liniowy. \n");
    else if (lx == 1)
        printf("Graf nie jest grafem liniowym. \n");

    return 1;
}
int sprawdzLin(int x,int y)
{
    for (x = 0; x <rozmMacierz; x++)
        for (y = x + 1; y < rozmMacierz; y++)
            for (int z = 0; z < rozmMacierz; z ++)
                if (macierz[z][x] == 1 && macierz[z][y] == 1)
                    return 1;
    return 0;
}


void przenumTab()
{
    int n = 1;

    for(int z=0; z < rozmMacierz; z++)
    {
        for (int k=0;  k < rozmMacierz; k++)
        {
            if (macierz[z][k] == 1)
                macierz[z][k] = n++;
        }
    }
    rozmMacierzOryg = n-1;

}


void generujGrafOryg()
{
    for(int w=0; w < rozmMacierz; w++)

        for (int k=0;  k < rozmMacierz; k++)
            if(macierz[w][k] > 0)
                wpiszWierzcholki(w, k);

}

void wpiszWierzcholki(int w, int k)
{
    int listaPoprz[MAX_TAB];
    int listaNast[MAX_TAB];
    int liczbaPoprz = 0;
    int liczbaNast = 0;
    int wartosc = 0;

    liczbaPoprz = poprzedniki(w, listaPoprz);
    liczbaNast  = nastepniki(k, listaNast);

    wartosc = macierz[w][k];

    if(liczbaPoprz > 0)
        for(int i = 0; i < liczbaPoprz; i++)
            macierzOryg[listaPoprz[i]-1][wartosc-1] = 1;
    else
    {
        macierzOryg[rozmMacierzOryg][wartosc-1] = 1;
        rozmMacierzOryg++;
    }
    if(liczbaNast > 0)
        for(int i = 0; i < liczbaNast; i++)
            macierzOryg[wartosc-1][listaNast[i]-1] = 1;
    else
    {
        macierzOryg[wartosc-1][rozmMacierzOryg] = 1;
        rozmMacierzOryg++;
    }
}

int poprzedniki(int kolumna, int listaPoprz[])
{
    int poprzedni = 0;

    for(int i=0; i < rozmMacierz; i++)
        if( macierz[i][kolumna] > 0 )
        {
            listaPoprz[poprzedni] = macierz[i][kolumna];
            poprzedni++;
        }
    return poprzedni;
}

int nastepniki(int wiersz, int listaNast[])
{
    int nastepny = 0;
    for(int i=0; i < rozmMacierz; i++)
        if( macierz[wiersz][i] > 0 )
        {
            listaNast[nastepny] = macierz[wiersz][i];
            nastepny++;
        }

    return nastepny;
}

void drukTab(int tryb)
{
    for(int w=0; w < rozmMacierz; w++)
        for (int k=0;  k < rozmMacierz; k++)
            if(tryb == 1)
            {
                if(macierz[w][k] != 0)
                    printf ("macierz[%d][%d]=%d\n", w, k, macierz[w][k]);
            }
            else
                printf ("macierz[%d][%d]=%d\n", w, k, macierz[w][k]);
}


void drukTabOryg(int tryb)
{
    printf("rozmMacierzOryg=%d\n", rozmMacierzOryg);
    for(int w=0; w < rozmMacierzOryg; w++)
        for (int k=0;  k < rozmMacierzOryg; k++)
            if(tryb == 1)
            {
                if(macierzOryg[w][k] != 0)
                    printf ("macierzOryg[%d][%d]=%d\n", w, k, macierzOryg[w][k]);
            }
            else
                printf ("macierzOryg[%d][%d]=%d\n", w, k, macierzOryg[w][k]);
}
