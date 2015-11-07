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
void drukTab();
void generujGrafOryg();
void wpiszWierzcholki(int x, int k);
void drukTabOryg();
int czyIstniejePoprzednik(int kolumna);
int czyIstniejeNastepnik(int wiersz);


int main()
{
    odczytajZPliku("c:/workcpacecodeb/Grafy_AKB/test.csv");
    czyJednograf();
    czySprzez();
    przenumTab();
    generujGrafOryg();
    drukTabOryg();

    return 0;
}

void odczytajZPliku(const char * nazwaPliku)
{
    char buflicz[10];
    fstream plik;
    string line;
    plik.open(nazwaPliku, ios::in);
    int linia = 0;
    int kolumna = 0;
    while(getline(plik,line))
    {
        const char * buf = line.c_str();
        int i = 0;
        int j = 0;
        //int liczKolumn = 0;
        linia++;
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
                macierz[linia-1][kolumna-1] = dana;
                printf("linia: %d kolumna: %d liczba: %d\n", linia, kolumna, dana);
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
    rozmMacierz = linia;
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

int sprawdzSprzez(int x, int y)
{
    int znaczRoz = 0;
    int znaczIden = 0;
    int z = 0;
    for (z =0; z < rozmMacierz; z ++)
        if ((macierz[x][z] == 0 && macierz[y][z] == 1))
            znaczRoz = 1;
        else if (macierz[x][z] == 1 && macierz[y][z] == 0)
            znaczRoz = 1;
        else if (macierz[x][z] == 1 && macierz[y][z] == 1)
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
    drukTab();
}


void drukTab()
{
    int z = 0;
    int k = 0;
    for(z=0; z < rozmMacierz; z++)
    {
        for (k=0;  k < rozmMacierz; k++)
        {
            printf ("macierz[%d][%d]=%d\n", z, k, macierz[z][k]);
        }
    }
}

void generujGrafOryg()
{

    int w = 0;
    int k = 0;

    for(int w=0; w < rozmMacierz; z++)
    {
        for (int k=0;  k < rozmMacierz; k++)
        {
            if(macierz[k][w] > 0)
                wpiszWierzcholki(z, w);
        }
    }
}

void wpiszWierzcholki(int z, int w)
{
    int poprzednik = 0;
    int nastepnik = 0;
    int wartosc = 0;

    poprzednik = czyIstniejePoprzednik(k);
    nastepnik  = czyIstniejeNastepnik(w);
    wartosc = macierz[k][w];

    if(poprzednik > 0)
        macierzOryg[poprzednik-1][wartosc-1] = 1;
    else
    {
        macierzOryg[rozmMacierzOryg][wartosc-1] = 1;
        rozmMacierzOryg++;
    }
    if(nastepnik > 0)
        macierzOryg[wartosc-1][nastepnik-1] = 1;
    else
    {
        macierzOryg[wartosc-1][rozmMacierzOryg] = 1;
        rozmMacierzOryg++;
    }
}

int czyIstniejePoprzednik(int kolumna)
{
    int i = 0;
    for(i=0; i < rozmMacierz; i++)
    {
        if( macierz[i][kolumna] > 0 )
            return macierz[i][kolumna];
    }
    return 0;
}

int czyIstniejeNastepnik(int wiersz)
{
    int i = 0;
    for(i=0; i < rozmMacierz; i++)
    {
        if( macierz[wiersz][i] > 0 )
            return macierz[wiersz][i];
    }
    return 0;

}

void drukTabOryg()
{
    int z = 0;
    int k = 0;
    printf("rozmMacierzOryg=%d\n", rozmMacierzOryg);
    for(z=0; z < rozmMacierzOryg; z++)
    {
        for (k=0;  k < rozmMacierzOryg; k++)
        {
            printf ("macierzOryg[%d][%d]=%d\n", z, k, macierzOryg[z][k]);
        }
    }
}
